#include "mywin.h"

#include "GrContext.h"
#include "SkCanvas.h"
#include "SkGradientShader.h"
#include "SkGraphics.h"

#include "deps/kutil/kconfig.h"
#include "graphy/layout.h"

using namespace sk_app;

Application* Application::Create(int argc, char** argv, void* platformData) {
    auto* app = new MyApp();
    app->doCreate(argc, argv, platformData);
    return app;
}

/*
void MyWin::onPaint(SkCanvas* canvas) {
    // Clear background
    canvas->clear(SK_ColorWHITE);

    SkPaint paint;
    paint.setColor(SK_ColorRED);

    // Draw a rectangle with red paint
    SkRect rect = SkRect::MakeXYWH(10, 10, 128, 128);
    canvas->drawRect(rect, paint);

    // Set up a linear gradient and draw a circle
    {
        SkPoint linearPoints[] = { { 0, 0 }, { 300, 300 } };
        SkColor linearColors[] = { SK_ColorGREEN, SK_ColorBLACK };
        paint.setShader(SkGradientShader::MakeLinear(linearPoints, linearColors, nullptr, 2,
                        SkShader::kMirror_TileMode));
        paint.setAntiAlias(true);

        canvas->drawCircle(200, 200, 64, paint);

        // Detach shader
        paint.setShader(nullptr);
    }

    // Draw a message with a nice black paint
    paint.setSubpixelText(true);
    paint.setColor(SK_ColorBLACK);
    paint.setTextSize(20);

    canvas->save();
    static const char message[] = "Hello World";

    // Translate and rotate
    canvas->translate(300, 300);
    fRotationAngle += 0.2f;
    if (fRotationAngle > 360) {
        fRotationAngle -= 360;
    }
    canvas->rotate(fRotationAngle);

    // Draw the text
    canvas->drawText(message, strlen(message), 0, 0, paint);

    canvas->restore();
}
*/

void MyWin::onCreated() {
    test::create_widgets((sia::graphy::Widget*)scene());
}

MyWin::MyWin() {
}

