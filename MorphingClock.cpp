#include "led-matrix.h"
#include "graphics.h"
#include "Digit.h"

#include <signal.h>
#include <unistd.h>
#include <iomanip>
#include <thread>
#include <iostream>
#include <string>

using namespace std;
using namespace rgb_matrix;
using std::chrono::system_clock;

RGBMatrix *canvas;

static void InterruptHandler(int signo) {
  canvas->Clear();
  delete canvas;
  
  printf("Received CTRL-C. Exiting.\n");
  exit(0);
}

static int usage(const char *progname, RGBMatrix::Options &matrix_options, rgb_matrix::RuntimeOptions &runtime_opt) {
  fprintf(stderr, "usage: %s [options]\n", progname);
  fprintf(stderr, "Displays the time as a series of falling Tetris blocks.\n");
  fprintf(stderr, "Options:\n");
  rgb_matrix::PrintMatrixFlags(stderr, matrix_options, runtime_opt);
  fprintf(stderr,
          "\t-x <x>            : Starting X position of displayed time. Default: 1\n"
          "\t-y <yFinish>      : Ending Y position of displayed time. Default: 16\n"
          "\t-0                : Show leading zeros in the hour.\n"
          "\t-t                : Use 24-hour clock.\n"
          "\t-r                : Force refresh of all digits every minute.\n"
          "\t-d                : Use double size blocks.\n"
          );
  return 1;
}

static int getDigit(string input, int pos)
{
  string individualNumber = input.substr(pos, 1);
  int number = (individualNumber != " ") ? stoi(individualNumber) : -1;
  return number;
}

int main(int argc, char *argv[]) {
  RGBMatrix::Options matrix_options;
  matrix_options.rows = 16;
  rgb_matrix::RuntimeOptions runtime_opt;
  if (!rgb_matrix::ParseOptionsFromFlags(&argc, &argv, &matrix_options, &runtime_opt)) {
    return usage(argv[0], matrix_options, runtime_opt);
  }

  char *time_format = "%I:%M";
  /*int x = 1;
  int yFinish = 16;
  bool leadingZero = false;
  bool forceRefresh = false;
  int scale = 1;

  int opt;
  while ((opt = getopt(argc, argv, "x:y:0trd")) != -1) {
    switch (opt) {
      case 'x':
        x = atoi(optarg);
        break;
      case 'y':
        yFinish = atoi(optarg);
        break;
      case '0':
        leadingZero = true;
        break;
      case 't':
        time_format = "%H:%M";
        break;
      case 'r':
        forceRefresh = true;
        break;
      case 'd':
        scale = 2;
        break;
      break;
      default:
        return usage(argv[0], matrix_options, runtime_opt);
    }
  }*/

  canvas = rgb_matrix::CreateMatrixFromOptions(matrix_options, runtime_opt);
  if (canvas == NULL)
    return 1;

  printf("Size: %dx%d. Hardware gpio mapping: %s\n", canvas->width(), canvas->height(), matrix_options.hardware_mapping);
  
  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);

  printf("Press <CTRL-C> to exit and reset LEDs\n");

  int segWidth = 4;

  Digit digit1(*canvas, 0, 1, 2, Color(0, 0, 255));
  Digit digit2(*canvas, 0, 1 + (segWidth + 3), 2, Color(0, 0, 255));
  Digit digit3(*canvas, 0, 1 + 3 + (segWidth + 3) * 2, 2, Color(0, 0, 255));
  Digit digit4(*canvas, 0, 1 + 3 + (segWidth + 3) * 3, 2, Color(0, 0, 255));

  digit3.DrawColon(Color(0, 0, 255));
  
  bool initialTime = true;
  
  while (true) {
      std::time_t tt = system_clock::to_time_t(system_clock::now());
      struct std::tm *ptm = std::localtime(&tt);
      std::stringstream ss;
      ss << std::put_time(ptm, time_format);
      string time = ss.str();
      
      /*if (!leadingZero && time[0] == '0')
      {
        time[0] = ' ';
      }*/
      
      //cout << time << endl;
        
      if (initialTime) { // If we didn't have a previous time. Just draw it without morphing.
        digit1.Draw(getDigit(time, 0));
        digit2.Draw(getDigit(time, 1));
        digit3.Draw(getDigit(time, 3));
        digit4.Draw(getDigit(time, 4));
        initialTime = false;
      }
      else
      {
        int h1 = getDigit(time, 0);
        int h2 = getDigit(time, 1);
        int m1 = getDigit(time, 3);
        int m2 = getDigit(time, 4);
        if (m2 != digit4.Value()) digit4.Morph(m2);
        if (m1 != digit3.Value()) digit3.Morph(m1);
        if (h2 != digit2.Value()) digit2.Morph(h2);
        if (h1 != digit1.Value()) digit1.Morph(h1);
      }

      ++ptm->tm_min;
      ptm->tm_sec=0;
      std::this_thread::sleep_until(system_clock::from_time_t(mktime(ptm)));
  }
  return 0;
}
