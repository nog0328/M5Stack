#include <M5Stack.h>

// 目的関数の例
double objectiveFunction(double x, double y) {
  return (x - 2.0) * (x - 2.0) + (y - 3.0) * (y - 3.0);
}

void setup() {
  M5.begin();
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(TFT_WHITE);
//  dacWrite(25, 0); // Speaker OFF
  
  int M=4;
  int N=2;
  
  // 初期単体の設定（4つの頂点を持つ）
  double initialSimplex[M][N] = {
    {0.0, 0.0},
    {1.0, 0.0},
    {0.0, 1.0},
    {1.0, 1.0}
  }; // (x - 2) * (x - 2) + (y - 3) * (y - 3) =

  double alpha = 2.0;  // 反射係数
  double beta  = 0.5;  // 収縮係数
  double gamma = 2.0;  // 拡張係数

  // 収束基準
  double tol = 1e-6;
  double xmin=0;
  double ymin=0;
  double funmin=0;
  double vertices[M][N]; // 初期単体の頂点と最終結果の格納用
  double fValues[M];
  double xBar = 0, yBar = 0;
  double xR,yR,fR;
  double xE,yE,fE;
  double xS,yS,fS;
  
  int iterations = 0;
  int maxIterations = 1000;
  int nR=0, nE=0, nS=0, nU=0;

//  M5.Lcd.printf("funmin01: %f\n", funmin);

  // Nelder-Mead法の実行
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
//      vertices[i][j] = initialSimplex[i][j]+0.01*random(-100,100);
      vertices[i][j] = initialSimplex[i][j];
    }
  }

  while (iterations < maxIterations) {
    // 目的関数の値を計算
    for (int i = 0; i < M; i++) {
      fValues[i] = objectiveFunction(vertices[i][0], vertices[i][1]);
    }

    // 頂点をソート
    for (int i = 0; i < M-1; i++) {
      for (int j = i + 1; j < M; j++) {
        if (fValues[j] < fValues[i]) {
          // fValuesを交換
          double tempF = fValues[i];
          fValues[i] = fValues[j];
          fValues[j] = tempF;

          // verticesを交換
          double tempX = vertices[i][0];
          vertices[i][0] = vertices[j][0];
          vertices[j][0] = tempX;

          double tempY = vertices[i][1];
          vertices[i][1] = vertices[j][1];
          vertices[j][1] = tempY;
        }
      }
    }

    // 収束判定
    if (fabs(fValues[3] - fValues[0]) < tol) {
      break;
    }
    Serial.printf("%d\n", random(-10,10));

    // 重心を計算
    for (int i = 0; i < M; i++) {
      xBar += vertices[i][0];
      yBar += vertices[i][1];
    }
    xBar /= 4.0;
    yBar /= 4.0;

    // 反射
//    xR = xBar + (alpha + 0.1*random(-10,10))*(xBar - vertices[3][0]);
//    yR = yBar + (alpha + 0.1*random(-10,10))*(yBar - vertices[3][1]);
    xR = xBar + alpha * (xBar - vertices[M-1][0]);
    yR = yBar + alpha * (yBar - vertices[M-1][1]);
    fR = objectiveFunction(xR, yR);
    
    if (fR < fValues[0]) {
      // 拡張
      xE = xBar + gamma * (xR - xBar);
      yE = yBar + gamma * (yR - yBar);
      fE = objectiveFunction(xE, yE);
      if (fE < fR) {
        vertices[M-1][0] = xE;
        vertices[M-1][1] = yE;
        nE++;
      } else {
        vertices[M-1][0] = xR;
        vertices[M-1][1] = yR;
        nR++;
      }
    } else if (fR < fValues[M-1]) {
      vertices[M-1][0] = xR;
      vertices[M-1][1] = yR;
      nR++;
    } else {
      // 収縮
      xS = xBar + beta * (vertices[M-1][0] - xBar);
      yS = yBar + beta * (vertices[M-1][1] - yBar);
      fS = objectiveFunction(xS, yS);
      if (fS < fValues[M-1]) {
        vertices[M-1][0] = xS;
        vertices[M-1][1] = yS;
        nS++;
      } else {
        // 収縮失敗の場合、単体を更新
        for (int i = 1; i < M; i++) {
          vertices[i][0] = vertices[0][0] + 0.5 * (vertices[i][0] - vertices[0][0]);
          vertices[i][1] = vertices[0][1] + 0.5 * (vertices[i][1] - vertices[0][1]);
          nU++;
        }
      }
    }

    iterations++;
  } // while

  // 結果を出力
  xmin = vertices[0][0];
  ymin = vertices[0][1];
  funmin = fValues[0];

//  M5.Lcd.setCursor(10, 10);
  M5.Lcd.printf("funmin: %.3f\n", funmin);
//  M5.Lcd.setCursor(10, 30);
  M5.Lcd.printf("xmin, ymin: %.3f, %.3f\n", xmin, ymin);
//  M5.Lcd.setCursor(10, 50);
  M5.Lcd.printf("iterations: %d\n", iterations);
  M5.Lcd.printf("fabs: %.3f\n",fabs(fValues[1] - fValues[0]));
  for (int i = 0; i < M; i++) {
  M5.Lcd.printf("fVals[%d] %.3f\n", i, fValues[i]);
  }
  M5.Lcd.printf("nR,nE,nS,nU: %d,%d,%d,%d\n", nR, nE, nS, nU);
} // setup

void loop() {
}
