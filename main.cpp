#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// 전역 변수 선언
Mat src, dst;
Point ptStart;          // 마우스 드래그 시작 좌표
bool isDrawing = false; // 마우스 드래그 상태 플래그

// 마우스 이벤트 처리 콜백 함수
void on_mouse(int event, int x, int y, int flags, void* userdata) {
    switch (event) {
    case EVENT_LBUTTONDOWN: // 1. 마우스 왼쪽 버튼 클릭
        ptStart = Point(x, y);
        isDrawing = true;
        break;

    case EVENT_MOUSEMOVE: // 2. 마우스 드래그 중 가이드선 표시
        if (isDrawing) {
            Mat temp = dst.clone();
            rectangle(temp, Rect(ptStart, Point(x, y)), Scalar(255), 1);
            imshow("src", temp);
        }
        break;

    case EVENT_LBUTTONUP: // 3. 마우스 버튼을 뗐을 때 필터 적용
        if (isDrawing) {
            isDrawing = false;

            // 드래그 영역 사각형 정의 및 이미지 경계 안전 처리
            Rect roi_rect(ptStart, Point(x, y));
            roi_rect &= Rect(0, 0, src.cols, src.rows);

            if (roi_rect.width > 0 && roi_rect.height > 0) {
                // 원본 영상(dst)에서 드래그한 영역(ROI) 추출 (참조 복사)
                Mat roi = dst(roi_rect);

                // [조건] 문제에서 제시된 3x3 샤프닝 마스크 정의
                float weights[9] = {
                    -1.0f, -1.0f, -1.0f,
                    -1.0f,  9.0f, -1.0f,
                    -1.0f, -1.0f, -1.0f
                };
                Mat sharpen_kernel(3, 3, CV_32FC1, weights);

                // ROI 영역에만 샤프닝 필터 적용 (마스크 총합이 1이므로 delta = 0)
                // 자기 자신(roi)을 입력과 출력으로 삼아 dst 원본에 바로 반영되게 합니다.
                filter2D(roi, roi, -1, sharpen_kernel, Point(-1, -1), 0);

                // 최종 업데이트된 이미지 화면에 출력
                imshow("src", dst);
            }
        }
        break;
    }
}

int main(void)
{
    // [조건] lenna 영상을 그레이스케일로 로드
    src = imread("lenna.bmp", IMREAD_GRAYSCALE);
    if (src.empty()) {
        cerr << "Image load failed!" << endl;
        return -1;
    }

    // 결과를 누적하여 갱신할 도화지 행렬 생성
    dst = src.clone();

    // 윈도우 창 생성 및 마우스 콜백 함수 설정
    namedWindow("src");
    setMouseCallback("src", on_mouse);

    // 최초 원본 영상 출력
    imshow("src", dst);

    waitKey(0);
    destroyAllWindows();

    return 0;
}