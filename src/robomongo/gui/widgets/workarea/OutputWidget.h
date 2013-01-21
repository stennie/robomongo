#pragma once

#include <QWidget>
#include <QLabel>
#include <QSplitter>

#include "robomongo/gui/editors/PlainJavaScriptEditor.h"
#include "robomongo/gui/widgets/workarea/BsonWidget.h"
#include "robomongo/core/domain/MongoShellResult.h"
#include "robomongo/gui/widgets/workarea/PagingWidget.h"

namespace Robomongo
{
    class OutputItemContentWidget;
    class OutputItemWidget;
    class OutputWidget;

    class OutputWidget : public QFrame
    {
        Q_OBJECT

    public:
        explicit OutputWidget(bool textMode, MongoShell *shell, QWidget *parent = 0);
        ~OutputWidget();

        void present(const QList<MongoShellResult> &documents);
        void updatePart(int partIndex, const MongoQueryInfo &queryInfo, const QList<MongoDocumentPtr> &documents);
        void toggleOrientation();

        void enterTreeMode();
        void enterTextMode();

        void maximizePart(OutputItemWidget *result);
        void restoreSize();

        int resultIndex(OutputItemWidget *result);

        void showProgress();
        void hideProgress();

        MongoShell *shell() const { return _shell; }

    private:
        QSplitter *_splitter;
        QLabel *_progressLabel;
        bool _textMode;
        MongoShell *_shell;
    };
}
