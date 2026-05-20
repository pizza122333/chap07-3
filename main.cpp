#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
    // 1. 영상 로드
    Mat src = imread("rose.bmp", IMREAD_GRAYSCALE);
    if (src.empty()) {
        cerr << "Image load failed!" << endl;
        return -1;
    }
    imshow("src", src);

    // [조건 1] 가우시안 필터 대신 평균값 필터를 사용하여 미리 흐린 영상 생성
    Mat blurred;
    blur(src, blurred, Size(3, 3)); // 3x3 평균값 필터 적용

    // [조건 2] alpha 값을 10부터 50까지 10씩 증가시키며 반복
    for (int alpha = 10; alpha <= 50; alpha += 10) {

        // 언샤프 마스크 공식을 이용한 수식 계산
        // dst = (1 + alpha) * src - alpha * blurred
        // OpenCV 내부적으로 수식 연산 시 자동으로 포화 연산(0~255 범위 제한)이 적용됩니다.
        Mat dst = (1 + (float)alpha) * src - (float)alpha * blurred;

        // 결과 영상에 현재 alpha 값 출력
        String desc = format("Mean Unsharp Mask: alpha = %d", alpha);
        putText(dst, desc, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.8,
            Scalar(255), 1, LINE_AA);

        // 결과 창 출력 (아무 키나 누르면 다음 alpha 값으로 진행)
        imshow("dst", dst);
        waitKey(0);
    }

    return 0;
}