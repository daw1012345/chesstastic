#include <iostream>
#include <chesstastic_lib/CameraCalibratorWidget.h>
#include <chesstastic_lib/CameraHandler.h>
#include <chesstastic_lib/CameraFeedback.h>
#include <chesstastic_lib/ChesstasticWindow.h>
#include <QApplication>
#include <QMainWindow>
#include <chesstastic_lib/ChessBoardWidget.h>
#include <chesstastic_lib/ChesstasticAPILinkingMiddleman.h>
#include <chesstastic_lib/ChesstasticAPIService.h>
#include <chesstastic_lib/ChesstasticLinkWidget.h>
#include <chesstastic_chess/ChessBoard.h>

int main(int argc, char **argv) {
    qRegisterMetaType<ChessBoard>("ChessBoard"); //
    qRegisterMetaType<ChessPieceSide>("ChessPieceSide");
    qRegisterMetaType<std::optional<ChessPieceSide>>("std::optional<ChessPieceSide>");
    qRegisterMetaType<std::map<ChessPieceSide,std::string>>("std::map<ChessPieceSide,std::string>");
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    auto detectionSettings = std::make_shared<CameraHandler>();
    if (argc > 1) {
        if (auto settings = detectionSettings.get()->get_mask().lock()) {
            auto[vars, _] = settings.get()->getMutableMask();
            vars.rotation = argv[1];
        }
    }


    detectionSettings->begin();
    detectionSettings->next_frame(); // Fetch first frame
    auto feedback = std::make_shared<CameraFeedback>(detectionSettings);
    feedback->run();

    ChesstasticWindow window(detectionSettings, feedback,
                             std::make_shared<ChesstasticNetworking::ChesstasticAPIService>());
    window.showMaximized();

    return app.exec();
}
