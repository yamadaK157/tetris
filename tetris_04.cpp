#include <iostream>  //入出力関連ヘッダ
#include <opencv2/opencv.hpp>  //OpenCV関連ヘッダ

#define x1 160  //
#define y1 50 //
#define x2 360  //
#define y2 450 //

#define width 20 //
#define height 20 //


int main (int argc, const char * argv[])
{
    //①カメラの初期化
    cv::VideoCapture capture(0);
    //cv::VideoCapture capture("print.mov");
    //カメラがオープンできたかどうかをチェック
    if (capture.isOpened()==0) {
        printf("Camera not found\n");
        return -1;
    }
    
    //カメラの画像取得サイズの指定
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    
    //②画像格納用インスタンス準備
    cv::Size imageSize(640, 480);
    cv::Mat frameImage(imageSize, CV_8UC3);
    cv::Mat backImage(imageSize, CV_8UC3);
    cv::Mat subImage(imageSize, CV_8UC3);
    cv::Mat subBinImage(imageSize, CV_8UC1);
    
    //動画ファイル書き出し準備
    cv::Mat recImage(cv::Size(640,480), CV_8UC3); //640x480, 3チャンネル
    //ビデオライタ生成
    cv::VideoWriter rec("rec.mpg", CV_FOURCC('P','I','M','1'), 30, recImage.size());
    
    //③ウィンドウの生成と移動
    cv::namedWindow("Frame");
    cv::moveWindow("Frame", 0,0);
    cv::namedWindow("Back");
    cv::moveWindow("Back", 50, 50);
    cv::namedWindow("Subtraction");
    cv::moveWindow("Subtraction", 100, 100);
    
    //動画画像処理無限ループ
    while(1){
        //④カメラから1フレーム読み込み
        capture >> frameImage;
        if(frameImage.data==0) break; //フレーム取得に失敗した場合には無限ループから脱出
        
//        //(b)"frameImage"と"backImage"との差分画像"subImage"の生成
//        cv::absdiff(frameImage, backImage, subImage);
//        
//        //(b')"subImage"をグレースケール化&しきい値処理して"subBinImage"生成
//        cv::cvtColor(subImage, subBinImage, CV_RGB2GRAY);
//        
//        cv::threshold(subBinImage, subBinImage, 150, 255, CV_THRESH_BINARY);
        
        //(b")"frameImage"をマスクとして使用して"recImaage"にコピー
        recImage = cv::Scalar(0);
        frameImage.copyTo(recImage, subBinImage);
        
        //枠線の描画, 座標, Red, 太さ3, 4近線連結
        cv::rectangle(frameImage, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 0, 255), 2, 4);
        
        //マス目の描画
        for(double j=y1; j<y2; j+=width){
            for(double i=x1; i<x2; i+=height){
                //unsigned char s = subBinImage.at<unsigned char>(j, i);
                cv::rectangle(frameImage, cv::Point(i, j), cv::Point(i+width, j+height), cv::Scalar(0, 0, 255), 2, 4);
            }
        }
        
        
        //動画ファイル書き出し
        rec << recImage; //ビデオライタに画像出力
        
        //⑥ウィンドウへの画像の表示
        cv::imshow("Frame", frameImage);
//        cv::imshow("Back", backImage);
//        cv::imshow("Subtraction", subBinImage);
        
        //キー入力待ち
        char key = cv::waitKey(30);//３０ミリ秒キー入力待ち
        if(key=='q') break;//[Q]キーが押されたら無限ループから脱出
    }
    
	//キー入力があるまでここでストップ
    cv::waitKey(0);

    //メッセージを出力して終了
	printf("Finished\n");
	return 0;
}
