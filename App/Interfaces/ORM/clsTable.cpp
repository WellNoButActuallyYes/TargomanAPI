/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   TargomanAPI is free software: you can redistribute it and/or modify
#   it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   TargomanAPI is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU AFFERO GENERAL PUBLIC LICENSE for more details.
#
#   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE
#   along with Targoman. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
/**
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 */

#include "clsTable.h"
#include "Interfaces/Common/HTTPExceptions.h"
#include "Interfaces/Common/GenericTypes.h"
#include "Interfaces/AAA/AAADefs.hpp"
#include <QRegularExpression>

namespace Targoman {
namespace API {
namespace ORM {

using namespace Targoman::DBManager;

QHash<QString, clsTable*> clsTable::Registry;
static bool TypesRegistered = false;

clsTable::clsTable(const QString& _schema,
                   const QString& _name,
                   const QList<clsORMField>& _cols,
                   const QList<stuRelation>& _foreignKeys) :
    Schema(_schema),
    Name(_name),
    BaseCols(_cols),
    ForeignKeys(_foreignKeys),
    CountOfPKs(0)
{
    clsTable::Registry.insert(Schema + "." + Name, this);
    if(TypesRegistered)
        return;

    TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI::Cols_t,
                                        QFieldValidator::allwaysValid(), _value,
                                        [](const QList<clsORMField>& _fields){
                                               QStringList Cols;
                                               foreach(auto Col, _fields)
                                                   if(Col.isVirtual() == false)
                                                        Cols.append(clsTable::finalColName(Col));
                                               return "Nothing for all or comma separated columns: (ex. "+Cols.first()+","+Cols.last()+") "
                                                      "you can also use aggregation functions: (ex. COUNT("+Cols.first()+"))\n"
                                                      "* COUNT\n* COUNT_DISTINCT\n* SUM\n* AVG\n* MAX\n* MIN\n\n"
                                                      "Availabale Cols are: \n* " + Cols.join("\n* ");
//                                               return "Nothing for all or comma separated columns: (ex. "+Cols.first()+","+Cols.last()+")";
                                        });
    TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI::Filter_t,
                                        QFieldValidator::allwaysValid(), _value,
                                        [](const QList<clsORMField>& _fields){
                                               return "Filtering rules where '+'=AND, '|'=OR, '*'=XOR. All parenthesis and logical operators must be bounded by space.\n"
                                                      "Equality/Inequality operators are\n"
                                                      "* =: equal\n"
                                                      "* !=: not equal\n"
                                                      "* <, <=, >, >=: inequal\n"
                                                      "* ~=: LIKE on strings\n"
                                                      "Take note that just columns listed in GroupBy field can be filtered\n"
                                                      "example: \n"
                                                      "* ( "+_fields.last().name()+"='1' | "+_fields.last().name()+"!='2' )";
                                        });
    TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI::OrderBy_t,
                                        QFieldValidator::allwaysValid(), _value,
                                        [](const QList<clsORMField>& _fields){
                                            QStringList Cols;
                                            foreach(auto Col, _fields)
                                                if(Col.isSortable() && Col.isVirtual() == false)
                                                    Cols.append(clsTable::finalColName(Col));
                                            return "Comma separated list of columns with +/- for ASC/DESC order prefix: (ex. +"+Cols.first()+",-"+Cols.last()+")\n* " + Cols.join("\n* ");
//                                            return "Comma separated list of columns with +/- for ASC/DESC order prefix: (ex. +"+Cols.first()+",-"+Cols.last()+")";
                                        });
    TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI::GroupBy_t,
                                        QFieldValidator::allwaysValid(), _value,
                                        [](const QList<clsORMField>& _fields){
                                               QStringList Cols;
                                               foreach(auto Col, _fields)
                                                    if(Col.isFilterable())
                                                        Cols.append(clsTable::finalColName(Col));
                                               return "Comma separated columns: \n* " + Cols.join(",\n* ");
//                                               return "Comma separated columns" ;
                                        });
    TypesRegistered = true;
}

QList<clsORMField> clsTable::filterItems(THttpMethod _method)
{
    QList<clsORMField> Filters;
    switch(_method){
    case qhttp::EHTTP_GET:
    case qhttp::EHTTP_DELETE:
        this->prepareFiltersList();
        foreach(auto Filter, this->BaseCols)
            if(Filter.isPrimaryKey())
                Filters.append(Filter);
        if(_method == qhttp::EHTTP_GET)
            Filters.append(clsORMField(COLS_KEY, S(Targoman::API::Cols_t), QFV, ORM_SELF_VIRTUAL));
        break;
    case qhttp::EHTTP_PATCH:
        foreach(auto Filter, this->BaseCols)
            if(Filter.isPrimaryKey() || Filter.isReadOnly() == false)
                Filters.append(Filter);
        break;
    case qhttp::EHTTP_PUT:
        foreach(auto Filter, this->BaseCols)
            if(Filter.isReadOnly() == false)
                Filters.append(Filter);
        break;
    default:
        Filters = this->AllCols;
        break;
    }
    return Filters;
}

void clsTable::updateFilterParamType(const QString& _fieldTypeName, QMetaType::Type _typeID)
{
    foreach(auto Col, this->BaseCols){
        if(Col.paramTypeName() == _fieldTypeName)
            Col.updateTypeID(_typeID);
    }
}

void clsTable::prepareFiltersList()
{
    if(this->AllCols.size())
        return;

    foreach(auto Col, this->BaseCols){
        if(Col.isPrimaryKey())
            ++this->CountOfPKs;

        QString FinalColName = this->finalColName(Col);
        clsORMField NewCol = clsORMField(Col, FinalColName);
        this->AllCols.append(NewCol);
        this->SelectableColsMap.insert(FinalColName, NewCol);
        if(Col.isFilterable())
            this->FilterableColsMap.insert(FinalColName, stuFilteredCol(NewCol));
        if(Col.isSortable())
            this->SortableColsMap.insert(FinalColName, NewCol);
    }

    foreach(auto Relation, this->ForeignKeys){
        clsTable* ForeignTable = this->Registry[Relation.ReferenceTable];
        if(ForeignTable == nullptr)
            throw exHTTPInternalServerError("Reference table has not been registered: " + Relation.ReferenceTable);

        foreach(auto Col, ForeignTable->BaseCols){
            QString FinalColName = this->finalColName(Col, Relation.RenamingPrefix);
            clsORMField NewCol = clsORMField(Col, FinalColName);
            this->AllCols.append(NewCol);
            this->SelectableColsMap.insert(FinalColName, NewCol);
            if(Col.isFilterable())
                this->FilterableColsMap.insert(FinalColName, stuFilteredCol(NewCol, Relation));
            if(Col.isSortable())
                this->SortableColsMap.insert(FinalColName, NewCol);
        }
    }

    foreach(auto Col, this->AllCols)
        if(Col.argSpecs().fromORMValueConverter() && !this->Converters.contains(this->finalColName(Col)))
            this->Converters.insert(this->finalColName(Col), Col.argSpecs().fromORMValueConverter());
}

QString clsTable::finalColName(const clsORMField& _col, const QString& _prefix){
    return _prefix + (_col.renameAs().isEmpty() ? _col.name() : _col.renameAs());
}

QString clsTable::makeColRenamedAs(const clsORMField& _col, const QString& _prefix) const {
    return (_col.renameAs().isEmpty() && _prefix.isEmpty() ? "" : " AS `"+ this->finalColName(_col, _prefix) + "`");
};

QString clsTable::makeColName(const clsORMField& _col, bool _appendAs, const stuRelation& _relation) const {
    return  (_relation.Column.isEmpty() ?
                 this->Name :
                 (_relation.RenamingPrefix.isEmpty() ?
                      _relation.ReferenceTable :
                      _relation.RenamingPrefix
                      )
                 ) + "." + _col.name() +
            (_appendAs ? this->makeColRenamedAs(_col, _relation.RenamingPrefix) : "");
};

QVariant clsTable::selectFromTable(DBManager::clsDAC& _db,
                                   const QStringList& _extraJoins,
                                   const QString& _extraFilters,
                                   const TAPI::ExtraPath_t& _extraPath,
                                   const TAPI::ORMFilters_t& _ORMFILTERS,
                                   quint64 _offset,
                                   quint16 _limit,
                                   QString _cols,
                                   const QString& _filters,
                                   const QString& _orderBy,
                                   const QString& _groupBy,
                                   bool _reportCount)
{
    this->prepareFiltersList();
    if(_ORMFILTERS.contains(COLS_KEY))
        _cols = _ORMFILTERS.value(COLS_KEY).toString();

    stuSelectItems SelectItems = this->makeListingQuery(_cols, _extraJoins, _filters + " " + _extraFilters, _orderBy, _groupBy);
    if(_extraPath.isEmpty()){
        TAPI::stuTable Table;

        if(this->BaseCols.last().name().endsWith("Status")){
            bool StatusFilterFound = false;
            foreach(auto Filter, SelectItems.Where)
                if(Filter.split(' ').contains(this->BaseCols.last().name())){
                    StatusFilterFound = true;
                    break;
                }
            if(StatusFilterFound == false)
                SelectItems.Where.append((SelectItems.Where.size() ? "AND " : " ") + this->BaseCols.last().name() + "!='R'");
        }

        Table.Rows = _db.execQuery("",
                                   QString("SELECT ")
                                   + (_reportCount ? "SQL_CALC_FOUND_ROWS" : "")
                                   + QUERY_SEPARATOR
                                   + SelectItems.Cols.join("," + QUERY_SEPARATOR)
                                   + QUERY_SEPARATOR
                                   + "FROM "
                                   + QUERY_SEPARATOR
                                   + SelectItems.From.join(QUERY_SEPARATOR)
                                   + QUERY_SEPARATOR
                                   + "WHERE "
                                   + SelectItems.Where.join(QUERY_SEPARATOR)
                                   + QUERY_SEPARATOR
                                   + (SelectItems.GroupBy.size() ? "GROUP BY " : "")
                                   + SelectItems.GroupBy.join(',')
                                   + QUERY_SEPARATOR
                                   + (SelectItems.OrderBy.size() ? "ORDER BY " : "")
                                   + SelectItems.OrderBy.join(',')
                                   + QUERY_SEPARATOR
                                   + QString("LIMIT %1,%2").arg(_offset).arg(_limit)
                                   ).toJson(false, this->Converters).toVariant().toList();
        if(_reportCount)
            Table.TotalRows = static_cast<qint64>(_db.execQuery("","SELECT FOUND_ROWS() AS cnt").toJson(true).object().value("cnt").toDouble());
        return Table.toVariant();
    }else{
        QStringList PrimaryKeyQueries = _extraPath.split(",");
        QStringList Filters;
        foreach(auto Query, PrimaryKeyQueries)
            foreach(auto Col, this->BaseCols)
                if(Col.isPrimaryKey()){
                    if(Query.size())
                        Filters.append(this->makeColName(Col) + " = \"" + Query + "\"");
                    break;
                }

        for(auto FilterIter = _ORMFILTERS.begin(); FilterIter != _ORMFILTERS.end(); ++FilterIter){
            if(FilterIter.key() == COLS_KEY) continue;
            const clsORMField& Col = this->SelectableColsMap.value(FilterIter.key());
            if(Col.isSelfIdentifier())
                Filters.append(this->makeColName(Col) + " = \"" + FilterIter.value().toString() + "\"");
        }

        QJsonDocument Result = _db.execQuery("",
                                             QString("SELECT ")
                                             + QUERY_SEPARATOR
                                             + SelectItems.Cols.join(',' + QUERY_SEPARATOR)
                                             + QUERY_SEPARATOR
                                             + "FROM "
                                             + QUERY_SEPARATOR
                                             + SelectItems.From.join(QUERY_SEPARATOR)
                                             + QUERY_SEPARATOR
                                             + "WHERE "
                                             + Filters.join(" AND ")
                                             + QUERY_SEPARATOR
                                             + "LIMIT 2" //Limit is set to 2 in roder to produce error if multi values are selected instead of one
                                             ).toJson(true, this->Converters);
        if(Result.object().isEmpty())
            throw exHTTPNotFound("No item could be found");

        return Result.toVariant();
    }
}

bool clsTable::update(DBManager::clsDAC& _db, const QVariantMap& _ORMFILTERS, QVariantMap _updateInfo)
{
    this->prepareFiltersList();
    if(_ORMFILTERS.isEmpty())
        throw exHTTPBadRequest("No filter has been set for update");

    QStringList  UpdateCommands;
    QVariantList Values;
    for(auto InfoIter = _updateInfo.begin(); InfoIter != _updateInfo.end(); ++InfoIter){
        if(InfoIter->isValid() == false)
            continue;
        clsORMField& Column = this->SelectableColsMap[InfoIter.key()];
        if(Column.isReadOnly())
            throw exHTTPInternalServerError("Invalid change to read-only column <" + InfoIter.key() + ">");

        Column.validate(InfoIter.value());
        UpdateCommands.append(this->makeColName(Column) + "=?");
        Values.append(Column.toDB(InfoIter.value().toString()));
    }

    QStringList  Filters;
    for(auto FilterIter = _ORMFILTERS.begin(); FilterIter != _ORMFILTERS.end(); FilterIter++){
        if(FilterIter->isValid() == false)
            continue;
        clsORMField& Column = this->SelectableColsMap[FilterIter.key()];
        if(Column.isReadOnly())
            continue;
        Column.validate(FilterIter.value());
        UpdateCommands.append(this->makeColName(Column) + "=?");
        Values.append(Column.toDB(FilterIter.value().toString()));
    }

    for(auto FilterIter = _ORMFILTERS.begin(); FilterIter != _ORMFILTERS.end(); FilterIter++){
        if(FilterIter->isValid() == false)
            continue;
        clsORMField& Column = this->SelectableColsMap[FilterIter.key()];
        if(Column.isFilterable() == false)
            throw exHTTPInternalServerError("Invalid non-filterable column <" + FilterIter.key() + ">");
        Filters.append(this->makeColName(Column) + "=?");
        Values.append(FilterIter.value());
    }

    clsDACResult Result = _db.execQuery("",
                                        QString("UPDATE ") + this->Schema + "." + this->Name
                                        + QUERY_SEPARATOR
                                        + "SET "
                                        + UpdateCommands.join("," + QUERY_SEPARATOR)
                                        + QUERY_SEPARATOR
                                        + "WHERE "
                                        + QUERY_SEPARATOR
                                        + Filters.join(" AND " + QUERY_SEPARATOR)
                                        ,Values);

    return Result.numRowsAffected() > 0;
}

bool clsTable::deleteByPKs(clsDAC& _db, TAPI::ExtraPath_t _EXTRAPATH, TAPI::ORMFilters_t _ORMFILTERS, bool _realDelete)
{
    this->prepareFiltersList();

    QStringList PrimaryKeyQueries = _EXTRAPATH.split(",");
    QStringList Filters;
    QVariantList Values;
    foreach(auto Query, PrimaryKeyQueries)
        foreach(auto Col, this->BaseCols)
            if(Col.isPrimaryKey()){
                if(Query.size())
                    _ORMFILTERS.insert(this->finalColName(Col), Query);
                break;
            }

    if(this->update(_db, _ORMFILTERS, {{this->BaseCols.last().name(), "Removed"}}) == false)
        return false;

    if(_realDelete == false)
        return true;

    for(auto FilterIter = _ORMFILTERS.begin(); FilterIter != _ORMFILTERS.end(); ++FilterIter){
        if(FilterIter->isValid() == false)
            continue;
        const clsORMField& Column = this->SelectableColsMap[FilterIter.key()];
        Filters.append(this->makeColName(Column) + "=?");
        Values.append(FilterIter.value());
    }

    clsDACResult Result = _db.execQuery("",
                                        QString("DELETE FROM ") + this->Schema + "." +this->Name
                                        + QUERY_SEPARATOR
                                        + "WHERE "
                                        + QUERY_SEPARATOR
                                        + Filters.join(" AND " + QUERY_SEPARATOR)
                                        ,Values);

    return Result.numRowsAffected() > 0;
}

QVariant clsTable::create(DBManager::clsDAC& _db, const QVariantMap& _ORMFILTERS, QVariantMap _createInfo)
{
    this->prepareFiltersList();
    QStringList  CreateCommands;
    QVariantList Values;
    for(auto InfoIter = _createInfo.begin(); InfoIter != _createInfo.end(); ++InfoIter){
        if(InfoIter->isValid() == false)
            continue;
        clsORMField& Col = this->SelectableColsMap[InfoIter.key()];
        if(Col.isReadOnly())
            throw exHTTPInternalServerError("Invalid change to read-only column <" + InfoIter.key() + ">");
        Col.validate(InfoIter.value());
        CreateCommands.append(this->makeColName(Col) + "=?");
        Values.append(InfoIter.value());
    }

    for(auto FilterIter = _ORMFILTERS.begin(); FilterIter != _ORMFILTERS.end(); FilterIter++){
        if(FilterIter->isValid() == false)
            continue;
        clsORMField& Col = this->SelectableColsMap[FilterIter.key()];
        if(Col.isReadOnly())
            continue;
        Col.validate(FilterIter.value());
        CreateCommands.append(this->makeColName(Col) + "=?");
        Values.append(Col.toDB(FilterIter.value().toString()));
    }

    clsDACResult Result = _db.execQuery("",
                                        QString("INSERT INTO ") + this->Name
                                        + QUERY_SEPARATOR
                                        + "SET "
                                        + CreateCommands.join("," + QUERY_SEPARATOR)
                                        + QUERY_SEPARATOR
                                        ,Values);

    return Result.lastInsertId();
}

void clsTable::setSelfFilters(const QVariantMap& _requiredFilters, TAPI::ExtraPath_t _EXTRAPATH, TAPI::ORMFilters_t& _ORMFILTERS, TAPI::Filter_t& _filters)
{
    if(_ORMFILTERS.size())
        return this->setSelfFilters(_requiredFilters, _EXTRAPATH, _ORMFILTERS);
    QStringList Filters;
    for(auto FilterIter = _requiredFilters.begin(); FilterIter != _requiredFilters.end(); ++FilterIter)
        Filters.append(QString("%1=%2").arg(FilterIter.key()).arg(FilterIter.value().toString()));
    _filters.insert(0, QString("( %1 )").arg(Filters.join(" + ")));
}

void clsTable::setSelfFilters(const QVariantMap& _requiredFilters, TAPI::ExtraPath_t _EXTRAPATH, TAPI::ORMFilters_t& _ORMFILTERS)
{
    Q_UNUSED(_EXTRAPATH);
    for(auto FilterIter = _requiredFilters.begin(); FilterIter != _requiredFilters.end(); ++FilterIter)
        _ORMFILTERS.insert(FilterIter.key(), FilterIter.value());
}



QStringList clsTable::privOn(qhttp::THttpMethod _method, QString _moduleName)
{
    QString CRUD;
    switch(_method){
    case qhttp::EHTTP_PUT:      CRUD = "T000";break;
    case qhttp::EHTTP_GET:      CRUD = "0T00";break;
    case qhttp::EHTTP_PATCH:    CRUD = "00T0";break;
    case qhttp::EHTTP_DELETE:   CRUD = "000T";break;
    default:
        Q_ASSERT_X(false, "privOn", "Invalid method on ORM");
    }
    return {_moduleName.replace("/", ":") + ":CRUD~" + CRUD };
}

clsTable::stuSelectItems clsTable::makeListingQuery(const QString& _requiredCols, const QStringList& _extraJoins, QString _filters, const QString& _orderBy, const QString _groupBy) const
{
    if(_requiredCols.size())
        QFV.asciiAlNum(false, ",()").validate(_requiredCols, "cols");

    QFV.optional(QFV.asciiAlNum(false, ",\\+\\-")).validate(_orderBy, "orderBy");
    QFV.optional(QFV.asciiAlNum(false, ",")).validate(_groupBy, "groupBy");

    clsTable::stuSelectItems SelectItems;

    /****************************************************************************/
    QStringList RequiredCols = _requiredCols.size() ? _requiredCols.split(",", QString::SkipEmptyParts) : QStringList("*");

    auto addCol = [this, _groupBy, RequiredCols](clsTable::stuSelectItems& _selectItems, auto _col, const stuRelation& _relation = InvalidRelation){
        foreach(auto RequiredCol, RequiredCols){
            QString ColName;
            QString Function;
            if(RequiredCol.contains('(')){
                if(RequiredCol.endsWith(')') == false)
                    throw exHTTPBadRequest("Aggroupation rule found without closing parenthesis: " + RequiredCol);
                Function = RequiredCol.split('(').first();
                ColName  = RequiredCol.split('(').last().replace(')', "");
                if(Function != "COUNT"
                   && Function != "COUNT_DISTINCT"
                   && Function != "SUM"
                   && Function != "AVG"
                   && Function != "MAX"
                   && Function != "MIN"
                   )
                    throw exHTTPBadRequest("Invalid agroupation rule: " + RequiredCol);
            }else{
                if(_groupBy.size())
                    Function = "ANY_VALUE";
                ColName = RequiredCol;
            }

            if(ColName == _relation.RenamingPrefix + _col.name () || ColName == _relation.RenamingPrefix + _col.renameAs()){
                if(Function.size()){
                    QString ColFinalName = this->makeColName(_col, true, _relation);
                    if(Function == "COUNT_DISTINCT")
                        _selectItems.Cols.append("COUNT(DISTINCT " + ColFinalName.split(' ').first()+ ") AS COUNT_" + ColFinalName.split(' ').last());
                    else
                        _selectItems.Cols.append(Function + "(" + ColFinalName.split(' ').first()+ ") AS " + Function + "_" + ColFinalName.split(' ').first().split('.').last());
                }else
                    _selectItems.Cols.append(this->makeColName(_col, true, _relation));
                return true;
            }
        }
        return false;
    };
    foreach(auto Col, this->BaseCols){
        if(RequiredCols.isEmpty() || RequiredCols.contains("*"))
            SelectItems.Cols.append(this->makeColName(Col, true));
        else
            addCol(SelectItems, Col);
    }

    QList<stuRelation> UsedJoins;
    foreach(auto Relation, this->ForeignKeys){
        clsTable* ForeignTable = this->Registry[Relation.ReferenceTable];
        if(ForeignTable == nullptr)
            throw exHTTPInternalServerError("Reference table has not been registered: " + Relation.ReferenceTable);

        bool Joined = false;
        foreach(auto Col, ForeignTable->BaseCols){
            if(RequiredCols.isEmpty() || RequiredCols.contains("*")){
                SelectItems.Cols.append(this->makeColName(Col, true, Relation));
                Joined = true;
            }else{
                if(addCol(SelectItems, Col, Relation))
                    Joined = true;
            }
        }

        if(Joined)
            UsedJoins.append(Relation);
    }

    if(SelectItems.Cols.isEmpty())
        throw exHTTPBadRequest("No columns found to be reported");

    if(RequiredCols.size() && RequiredCols.size() > SelectItems.Cols.size())
        throw exHTTPBadRequest("Seems that some columns could not be resolved: Actives are: [" +SelectItems.Cols.join(", ")+ "]");

    /****************************************************************************/
    quint8 OpenParenthesis = 0;
    bool CanStartWithLogical = false;
    QString LastLogical = "";
    _filters = _filters.replace("\\ ", "$SPACE$");
    foreach(auto Filter, _filters.split(" ", QString::SkipEmptyParts)){
        QString Rule;
        Filter = Filter.trimmed ();
        if(Filter == ")"){
            if(OpenParenthesis <= 0) throw exHTTPBadRequest("Invalid close parenthesis without any open");
            Rule = " )";
            OpenParenthesis--;
            CanStartWithLogical = true;
        }else if(Filter == "("){
            Rule = LastLogical + "(";
            CanStartWithLogical = false;
            LastLogical.clear();
            OpenParenthesis++;
        }else if(Filter == '+' || Filter == '|' || Filter == '*'){
            if(CanStartWithLogical == false) throw exHTTPBadRequest("Invalid logical expression prior to any rule");
            if(Filter == '+') LastLogical = "AND ";
            else if(Filter == '|') LastLogical = "OR ";
            else if(Filter == '*') LastLogical = "XOR ";

            CanStartWithLogical = false;
            continue;
        }else{
            static QRegularExpression rxFilterPattern("([a-zA-Z0-9\\_]+)([<>!=~]=?)(.+)");
            Filter = Filter.replace("$SPACE$", " ");
            QRegularExpressionMatch PatternMatches = rxFilterPattern.match(Filter);
            if(PatternMatches.lastCapturedIndex() != 3)
                throw exHTTPBadRequest("Invalid filter set: " + Filter);

            Rule = LastLogical;

            stuFilteredCol FilteredCol = this->FilterableColsMap.value(PatternMatches.captured(1).trimmed());
            if(FilteredCol.isValid())
                Rule+=this->makeColName(FilteredCol.Col, false, FilteredCol.Relation);
            else
                throw exHTTPBadRequest("Invalid column for filtring: " + PatternMatches.captured(1));

            if(FilteredCol.Relation.Column.size() && UsedJoins.contains(FilteredCol.Relation) == false)
                UsedJoins.append(FilteredCol.Relation);

            if(PatternMatches.captured(3) == "NULL"){
                if(PatternMatches.captured(2) == "=")
                    Rule += " IS NULL";
                else if(PatternMatches.captured(2) == "!=")
                    Rule += " IS NOT NULL";
                else
                    throw exHTTPBadRequest("Invalid filter with NULL expression: " + Filter);

                SelectItems.Where.append(Rule);
                CanStartWithLogical = true;
                LastLogical.clear();
                continue;
            }

            if(PatternMatches.captured(2) == "<") Rule += " < ";
            else if(PatternMatches.captured(2) == "<=") Rule += " <= ";
            else if(PatternMatches.captured(2) == ">") Rule += " > ";
            else if(PatternMatches.captured(2) == ">=") Rule += " >= ";
            else if(PatternMatches.captured(2) == "!=") Rule += " != ";
            else if(PatternMatches.captured(2) == "~=") Rule += " LIKE ";
            else if(PatternMatches.captured(2) == "=") Rule += " = ";
            else throw exHTTPBadRequest("Invalid filter criteria: " + Filter);

            FilteredCol.Col.argSpecs().validate(PatternMatches.captured(3), PatternMatches.captured(1).trimmed().toLatin1());

            Rule += FilteredCol.Col.argSpecs().isPrimitiveType() ? "" : "'";
            Rule += FilteredCol.Col.toDB(PatternMatches.captured(3)).toString();
            Rule += FilteredCol.Col.argSpecs().isPrimitiveType() ? "" : "'";
            CanStartWithLogical = true;
            LastLogical.clear();
        }
        SelectItems.Where.append(Rule);
    }

    if(OpenParenthesis != 0)
        throw exHTTPBadRequest("count of open and closed parenthesises does not match");

    if(SelectItems.Where.isEmpty())
        SelectItems.Where.append("TRUE");

    /****************************************************************************/

    SelectItems.From.append(this->Schema + "." + this->Name);
    foreach(auto Join, UsedJoins){
        SelectItems.From.append((Join.LeftJoin ? "LEFT JOIN " : "JOIN ")
                                + Join.ReferenceTable
                                + (Join.RenamingPrefix.size() ? " `" + Join.RenamingPrefix + "`" : "")
                                + " ON "
                                + (Join.RenamingPrefix.size() ? "`" + Join.RenamingPrefix + "`" : Join.ReferenceTable) + "." + Join.ForeignColumn
                                + " = "
                                + this->Name + "." + Join.Column
                                );
    }

    if(_extraJoins.size())
        SelectItems.From.append(_extraJoins);

    /****************************************************************************/
    foreach(auto OrderByCriteria, _orderBy.split(",", QString::SkipEmptyParts)) {
        QString Direction = "ASC";
        if(OrderByCriteria.startsWith("-")){
            Direction = "DESC";
            OrderByCriteria = OrderByCriteria.mid(1);
        }else if(OrderByCriteria.startsWith("+"))
            OrderByCriteria = OrderByCriteria.mid(1);

        if(this->SortableColsMap.contains(OrderByCriteria) == false)
            throw exHTTPBadRequest("Invalid Order by <"+OrderByCriteria+"> not found in columns");

        SelectItems.OrderBy.append(OrderByCriteria + " " + Direction);
    }

    /****************************************************************************/
    foreach(auto GroupByCriteria, _groupBy.split(",", QString::SkipEmptyParts)) {
        stuFilteredCol Filter = this->FilterableColsMap.value(GroupByCriteria.trimmed());
        if(Filter.isValid())
            SelectItems.GroupBy.append(this->makeColName(Filter.Col, false, Filter.Relation));
        else
            throw exHTTPBadRequest("Invalid group by <"+GroupByCriteria+"> not found in columns");
    }

    return SelectItems;
}


}
}
}