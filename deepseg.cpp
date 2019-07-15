#include <QtCore>
#include <QImage>

#include <iostream>

#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/mutable_op_resolver.h"
#include "tensorflow/lite/optional_debug_tools.h"

#define CHECK(x)                                                   \
    if (!(x)) {                                                    \
        qDebug() << "Error at" << __FILE__ << ":" << __LINE__ ;    \
        exit(1) ;                                                  \
    }

#define NORM(x)  (float(x) - 127.5f) / 255.0f

#define INPW 257 // input width
#define INPH 257 // input height
#define INPC 3   // number of input channels, (r,g,b)
#define OUTC 21  // number of output channels
#define SELC 15  // 출력값으로 사용할 채널 번호, 13은 프로필


static std::unique_ptr<tflite::FlatBufferModel> model ;
static const char* fnmodel = "/home/lhk/test/ml/deeplab/app/src/main/assets/deeplabv3_257_mv_gpu.tflite" ;

QImage deepseg(const QImage& imgIn) {
    if (imgIn.isNull() || imgIn.depth() != 32) return QImage() ;

    QImage img = imgIn.scaled(INPW, INPW) ;

    if (!model) {
        model = tflite::FlatBufferModel::BuildFromFile(fnmodel) ;
        assert(model) ;
    }

    tflite::ops::builtin::BuiltinOpResolver resolver ;
    std::unique_ptr<tflite::Interpreter> interpreter ;
    tflite::InterpreterBuilder(*model, resolver)(&interpreter) ;
    CHECK(interpreter) ;
    interpreter->UseNNAPI(false) ;

    TfLiteStatus st = interpreter->AllocateTensors() ;
    CHECK(st == kTfLiteOk) ;

    // fill input buffers
    {
        float *q = interpreter->typed_input_tensor<float>(0) ;
        for (int r = 0 ; r < INPH ; r++) {
            const unsigned char* p = img.scanLine(r) ;  // assume ABGR
            int nc = INPW ;
            while (nc--) {
                *q++ = NORM(*p++) ;  // red
                *q++ = NORM(*p++) ;  // green
                *q++ = NORM(*p) ;    // blue
                p += 2 ;             // alpha, skip
            }
        }
    }

    // run inference
    st = interpreter->Invoke() ;
    CHECK(st == kTfLiteOk) ;

    // logit softmax over output channels
    QImage mask(img.size(), QImage::Format_Grayscale8) ;
    {
        const float* output = interpreter->typed_output_tensor<float>(0) ;

        for (int r = 0 ; r < INPH ; r++) {
            const float* p = output + INPW * OUTC * r ;
            unsigned char* q = mask.scanLine(r) ;
            int nc = INPW ;
            while (nc--) {
                // find max value
                const float* p1 = p ;
                double pmax = double(*p1++) ;
                int nchan = OUTC - 1 ;
                while (nchan--) {
                    double v = double(*p1++) ;
                    if (v > pmax) pmax = v ;
                }

                // softmax sum
                double s = 0 ;
                p1 = p ;
                nchan = OUTC ;
                while (nchan--) s += exp(double(*p1++) - pmax) ;
                *q++ = (unsigned char)(exp(double(p[SELC]) - pmax) / s * 255) ;

                p += OUTC ;
            }
        }
    }

    return mask ;
}
