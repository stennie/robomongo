#ifndef MONGOEVENTS_H
#define MONGOEVENTS_H

#include <QString>
#include <QStringList>
#include <QEvent>
#include "Core.h"
#include "mongo/client/dbclient.h"
#include <engine/Result.h>
#include "domain/MongoShellResult.h"

namespace Robomongo
{
    /**
     * @brief Error class
     */
    class Error
    {
    public:
        Error() :
            isNull(true) {}

        Error(const QString &errorMessage) :
            isNull(false),
            errorMessage(errorMessage) {}

        QString errorMessage;
        bool isNull;
    };

    class REvent : public QEvent
    {
    public:
        REvent(QEvent::Type type) :
            QEvent(type) {}

        virtual const char *typeString() = 0;
    };

    /**
     * @brief Request & Response
     */

    class Request : public REvent
    {
    public:
        Request(QEvent::Type type, QObject *sender) :
            REvent(type),
            sender(sender) {}

        QObject *sender;
        virtual const char *typeString() = 0;
    };

    class Response : public REvent
    {
    public:
        Response(QEvent::Type type) :
            REvent(type) {}

        Response(QEvent::Type type, Error error) :
            REvent(type),
            error(error) {}

        bool isError() const { return !error.isNull; }
        QString errorMessage() const { return error.errorMessage; }

        virtual const char *typeString() = 0;
        Error error;
    };

    /**
     * @brief Init Request & Response
     */

    class InitRequest : public Request
    {
        R_MESSAGE

        InitRequest(QObject *sender) :
            Request(EventType, sender) {}
    };

    class InitResponse : public Response
    {
        R_MESSAGE

        InitResponse() :
            Response(EventType) {}

        InitResponse(const Error &error) :
            Response(EventType, error) {}
    };

    /**
     * @brief Init Request & Response
     */

    class FinalizeRequest : public Request
    {
        R_MESSAGE

        FinalizeRequest(QObject *sender) :
            Request(EventType, sender) {}
    };

    class FinalizeResponse : public Response
    {
        R_MESSAGE

        FinalizeResponse() :
            Response(EventType) {}

        FinalizeResponse(const Error &error) :
            Response(EventType, error) {}
    };


    /**
     * @brief EstablishConnection
     */

    class EstablishConnectionRequest : public Request
    {
        R_MESSAGE

        EstablishConnectionRequest(QObject *sender, const QString &databaseName,
                                   const QString &userName, const QString &userPassword) :
            Request(EventType, sender),
            databaseName(databaseName),
            userName(userName),
            userPassword(userPassword) {}

        QString databaseName;
        QString userName;
        QString userPassword;
    };

    class EstablishConnectionResponse : public Response
    {
        R_MESSAGE

        EstablishConnectionResponse(const QString &address) :
            Response(EventType),
            address(address) {}

        EstablishConnectionResponse(const Error &error) :
            Response(EventType, error) {}

        QString address;
    };


    /**
     * @brief LoadDatabaseNames
     */

    class LoadDatabaseNamesRequest : public Request
    {
        R_MESSAGE

        LoadDatabaseNamesRequest(QObject *sender) :
            Request(EventType, sender) {}
    };

    class LoadDatabaseNamesResponse : public Response
    {
        R_MESSAGE

        LoadDatabaseNamesResponse(const QStringList &databaseNames) :
            Response(EventType),
            databaseNames(databaseNames) {}

        LoadDatabaseNamesResponse(const Error &error) :
            Response(EventType, error) {}

        QStringList databaseNames;
    };


    /**
     * @brief LoadCollectionNames
     */

    class LoadCollectionNamesRequest : public Request
    {
        R_MESSAGE

        LoadCollectionNamesRequest(QObject *sender, const QString &databaseName) :
            Request(EventType, sender),
            databaseName(databaseName) {}

        QString databaseName;
    };

    class LoadCollectionNamesResponse : public Response
    {
        R_MESSAGE

        LoadCollectionNamesResponse(const QString &databaseName, const QStringList &collectionNames) :
            Response(EventType),
            databaseName(databaseName),
            collectionNames(collectionNames) { }

        LoadCollectionNamesResponse(const Error &error) :
            Response(EventType, error) {}

        QString databaseName;
        QStringList collectionNames;
    };


    /**
     * @brief Query Mongodb
     */

    class ExecuteQueryRequest : public Request
    {
        R_MESSAGE

        ExecuteQueryRequest(QObject *sender, const QString &nspace, int take = 0, int skip = 0) :
            Request(EventType, sender),
            nspace(nspace),
            take(take),
            skip(skip) {}

        QString nspace; //namespace of object (i.e. "database_name.collection_name")
        int take; //
        int skip;
    };

    class ExecuteQueryResponse : public Response
    {
        R_MESSAGE

        ExecuteQueryResponse(const QList<mongo::BSONObj> &documents) :
            Response(EventType),
            documents(documents) { }

        ExecuteQueryResponse(const Error &error) :
            Response(EventType, error) {}

        QList<mongo::BSONObj> documents;
    };


    /**
     * @brief ExecuteScript
     */

    class ExecuteScriptRequest : public Request
    {
        R_MESSAGE

        ExecuteScriptRequest(QObject *sender, const QString &script, const QString &dbName, int take = 0, int skip = 0) :
            Request(EventType, sender),
            script(script),
            databaseName(dbName),
            take(take),
            skip(skip) {}

        QString script;
        QString databaseName;
        int take; //
        int skip;
    };

    class ExecuteScriptResponse : public Response
    {
        R_MESSAGE

        ExecuteScriptResponse(const QList<Result> &results) :
            Response(EventType),
            results(results) { }

        ExecuteScriptResponse(const Error &error) :
            Response(EventType, error) {}

        QList<Result> results;
    };



    class SomethingHappened : public REvent
    {
        R_MESSAGE

        SomethingHappened(const QString &something) :
            REvent(EventType),
            something(something) { }

        QString something;
    };

    class ConnectingEvent : public REvent
    {
        R_MESSAGE

        ConnectingEvent(const MongoServerPtr &server) :
            REvent(EventType),
            server(server) { }

        MongoServerPtr server;
    };

    class OpeningShellEvent : public REvent
    {
        R_MESSAGE

        OpeningShellEvent(const MongoShellPtr &shell, const QString &initialScript) :
            REvent(EventType),
            shell(shell),
            initialScript(initialScript) { }

        MongoShellPtr shell;
        QString initialScript;
    };

    class ConnectionFailedEvent : public REvent
    {
        R_MESSAGE

        ConnectionFailedEvent(const MongoServerPtr &server) :
            REvent(EventType),
            server(server) { }

        MongoServerPtr server;
    };

    class ConnectionEstablishedEvent : public REvent
    {
        R_MESSAGE

        ConnectionEstablishedEvent(const MongoServerPtr &server) :
            REvent(EventType),
            server(server) { }

        ~ConnectionEstablishedEvent() {
            int a = 56;
        }

        MongoServerPtr server;
    };

    class DatabaseListLoadedEvent : public REvent
    {
        R_MESSAGE

        DatabaseListLoadedEvent(const QList<MongoDatabasePtr> &list) :
            REvent(EventType),
            list(list) { }

        QList<MongoDatabasePtr> list;
    };

    class DocumentListLoadedEvent : public REvent
    {
        R_MESSAGE

        DocumentListLoadedEvent(const QString &query, const QList<MongoDocumentPtr> &list) :
            REvent(EventType),
            query(query),
            list(list) { }

        QList<MongoDocumentPtr> list;
        QString query;
    };

    class ScriptExecutedEvent : public REvent
    {
        R_MESSAGE

        ScriptExecutedEvent(const QList<MongoShellResult> &list) :
            REvent(EventType),
            results(list) { }

        QList<MongoShellResult> results;
    };

}

#endif // MONGOEVENTS_H
