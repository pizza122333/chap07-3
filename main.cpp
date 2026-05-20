#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// 전역 변수 선언 (콜백 함수에서 접근하기 위함)
Mat src, blurred;
int alpha_val = 1;       // [조건] 알파 초기값 (예시로 1 설정)
const int max_val = 50;  // [조건] 트랙바 최대 범위 50

// 트랙바 값이 변경될 때마다 호출되는 콜백 함수
void on_trackbar(int pos, void*) {
    // 언샤프 마스크 수식 계산
    // dst = (1 + alpha) * src - alpha * blurred
    // 수식 연산 시 OpenCV가 자동으로 포화 연산(0~255 범위 제한)을 수행합니다.
    Mat dst = (1 + (float)alpha_val) * src - (float)alpha_val * blurred;

    // 결과 영상에 현재 alpha 값 글자 출력
    String desc = format("Mean Unsharp Mask: alpha = %d", alpha_val);
    putText(dst, desc, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.8,
        Scalar(255), 1, LINE_AA);

    // 가중치가 적용된 결과 화면 갱신
    imshow("dst", dst);
}

int main(void)
{
    // 1. 영상 로드
    src = imread("rose.bmp", IMREAD_GRAYSCALE);
    if (src.empty()) {
        cerr << "Image load failed!" << endl;
        return -1;
    }
    imshow("src", src);

    // [실습과제 2번 조건] 가우시안 대신 평균값 필터를 사용하여 흐린 영상 생성
    blur(src, blurred, Size(3, 3)); // 3x3 크기의 평균값 블러 적용

    // 결과 출력을 위한 윈도우 창 생성
    namedWindow("dst");

    // 2. [조건] 트랙바 생성 (트랙바 이름, 윈도우 창 이름, 등록할 변수 주소, 최대값, 콜백함수)
    createTrackbar("alpha", "dst", &alpha_val, max_val, on_trackbar);

    // 초기값(alpha_val = 1)을 기준으로 첫 화면을 강제 렌더링하기 위해 콜백 함수 1회 직접 호출
    on_trackbar(alpha_val, 0);

    // 사용자가 키를 누를 때까지 프로그램 유지
    waitKey(0);
    destroyAllWindows();

    return 0;
}