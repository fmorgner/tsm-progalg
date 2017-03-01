#include "FreeImagePlus.h"
#include "Stopwatch.h"

#include <array>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>

#ifndef WIN32
typedef DWORD COLORREF;

/*
   typedef struct tagRGBQUAD {
   BYTE    rgbBlue;
   BYTE    rgbGreen;
   BYTE    rgbRed;
   BYTE    rgbReserved;
   } RGBQUAD;
   */
#endif

BYTE dist(int x, int y)
  {
  int d = (int)std::hypot(x, y);
  return (d < 256) ? d : 255;
  }

void processSerial(const fipImage& input, fipImage& output)
  {
  assert(input.getWidth() == output.getWidth() &&
         input.getHeight() == output.getHeight() &&
         input.getImageSize() == output.getImageSize() &&
         input.getBitsPerPixel() == 32);

  auto constexpr fSize = 3;
  auto constexpr fSize2 = fSize/2;
  auto constexpr hFilter = std::array<std::array<int, fSize>, fSize> {{
      {{  1,  1,  1 }},
      {{  0,  0,  0 }},
      {{ -1, -1, -1 }},
  }};
  auto constexpr vFilter = std::array<std::array<int, fSize>, fSize> {{
      {{ 1, 0, -1 }},
      {{ 1, 0, -1 }},
      {{ 1, 0, -1 }},
  }};

  for (unsigned int v = fSize2; v < output.getHeight() - fSize2; v++)
    {
    for (unsigned int u = fSize2; u < output.getWidth() - fSize2; u++)
      {
      RGBQUAD iC;
      int hC[3] = { 0, 0, 0 };
      int vC[3] = { 0, 0, 0 };

      for (unsigned int j = 0; j < fSize; j++)
        {
        for (unsigned int i = 0; i < fSize; i++)
          {
          input.getPixelColor(u + i - fSize2, v + j - fSize2, &iC);
          hC[0] += hFilter[j][i]*iC.rgbBlue;
          vC[0] += vFilter[j][i]*iC.rgbBlue;
          hC[1] += hFilter[j][i]*iC.rgbGreen;
          vC[1] += vFilter[j][i]*iC.rgbGreen;
          hC[2] += hFilter[j][i]*iC.rgbRed;
          vC[2] += vFilter[j][i]*iC.rgbRed;
          }
        }
      RGBQUAD oC = { dist(hC[0], vC[0]), dist(hC[1], vC[1]), dist(hC[2], vC[2]), 255 };
      output.setPixelColor(u, v, &oC);
      }
    }
  }

static void processSerialOpt(const fipImage& input, fipImage& output)
  {
  int constexpr bypp = 4;
  assert(input.getWidth() == output.getWidth() &&
         input.getHeight() == output.getHeight() &&
         input.getImageSize() == output.getImageSize() &&
         input.getBitsPerPixel() == bypp*8);

  // TODO
  }

static void processParallel(const fipImage& input, fipImage& output)
  {
  int constexpr bypp = 4;
  assert(input.getWidth() == output.getWidth() &&
         input.getHeight() == output.getHeight() &&
         input.getImageSize() == output.getImageSize() &&
         input.getBitsPerPixel() == bypp*8);

  // TODO
  }

bool operator==(const fipImage& im1, const fipImage& im2)
  {
  assert(im1.getWidth() == im2.getWidth() &&
         im1.getHeight() == im2.getHeight() &&
         im1.getImageSize() == im2.getImageSize() &&
         im1.getBitsPerPixel() == 32);

  for(unsigned int i = 0; i < im1.getHeight(); i++)
    {
    COLORREF *row1 = reinterpret_cast<COLORREF*>(im1.getScanLine(i));
    COLORREF *row2 = reinterpret_cast<COLORREF*>(im2.getScanLine(i));
    for(unsigned int j = 0; j < im1.getWidth(); j++)
      {
      if (row1[j] != row2[j]) return false;
      }
    }

  return true;
}

bool operator!=(const fipImage& im1, const fipImage& im2)
  {
  return !(im1 == im2);
  }

int imageProcessing(int argc, const char* argv[])
  {
  std::cout << "Image processing started\n";

  if (argc < 3)
    {
    std::cerr << "Usage: " << argv[0] << " input-file-name output-file-name\n";
    return -1;
    }

  Stopwatch sw{};
  fipImage image{};

  if (!image.load(argv[1]))
    {
    std::cerr << "Image not found: " << argv[1] << '\n';
    return -1;
    }

  // create output images
  fipImage out1(image), out2(image), out3(image);

  // process image sequentially and produce out1
  std::cout << "Start sequential process" << '\n';
  sw.Start();
  processSerial(image, out1);
  sw.Stop();
  double seqTime = sw.GetElapsedTimeMilliseconds();
  std::cout << seqTime << " ms" << '\n';

  // process image sequentially but optimized and produce out2
  std::cout << "Start optimized sequential process" << '\n';
  sw.Start();
  processSerialOpt(image, out2);
  sw.Stop();
  double seqOptTime = sw.GetElapsedTimeMilliseconds();
  std::cout << seqOptTime << " ms, speedup = " << seqTime/seqOptTime << '\n';

  // compare out1 with out2
  std::cout << std::boolalpha << "The two operations produce the same results: " << (out1 == out2) << '\n';

  // process image in parallel and produce out3
  std::cout << "Start parallel process" << '\n';
  sw.Start();
  processParallel(image, out3);
  sw.Stop();
  std::cout << sw.GetElapsedTimeMilliseconds() << " ms, speedup = " << seqOptTime/sw.GetElapsedTimeMilliseconds() << '\n';

  // compare out1 with out3
  std::cout << std::boolalpha << "The two operations produce the same results: " << (out1 == out3) << '\n';

  // save output image
  if (!out1.save(argv[2])) {
    std::cerr << "Image not saved: " << argv[2] << '\n';
    return -1;
  }

  return 0;
}
