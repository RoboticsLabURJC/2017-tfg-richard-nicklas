// imgui headers
#include <SDL.h>
#include <SDL_opengl.h>
#include <assert.h>
#include <stdio.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#ifndef ASSETS_DIR
#error ASSETS_DIR undefined
#endif

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

// verilator headers
#include "Vdesign_top.h"
#include "Vdesign_top___024root.h"
#include "verilated.h"
#include "verilated_vcd_c.h"


//ROS & opencv
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

#include <Conversor.h>

#define LED_ICON "\uf111"
#define START_ICON "\uf04b"
#define STOP_ICON "\uf04d"
#define STEP_ICON "\uf051"
#define RESET_ICON "\uf0e2"

typedef cv::Vec<uint8_t, 3> BGRPixel;
typedef cv::Vec<uint8_t, 4> BGRAPixel;

const int n_pos_leds = 8;
const int n_dis_leds = 3;
const int cols = 80;
const int rows = 60;
const uint8_t ALPHA_SOLID = 255;

const char input_image_1_path[] = ASSETS_DIR "/red_ball_center_80x60.png";

const char font_awesome_path[] = ASSETS_DIR "/fa-solid-900.ttf";

class SimElement {
 public:
  virtual ~SimElement() {}
  virtual void onReset() {}
  virtual void postReset() {}
  virtual void preCycle() {}
  virtual void postCycle() {}
};

class InputDriver : public SimElement {
 public:
  InputDriver(Vdesign_top *top, const uint8_t *wRgbfilter,
              const cv::Mat **input_image)
      : top{top},
        wRgbfilter{wRgbfilter},
        input_image{input_image},
        input_addr{0} {}

  virtual ~InputDriver() {}

  virtual void onReset() { this->input_addr = 0; }

  virtual void preCycle() {}

  virtual void postCycle() {
    auto image_data = (*this->input_image)->ptr<BGRPixel>();
    auto px = image_data[this->input_addr];

    this->top->addrin = this->input_addr;
    this->top->wea = 1;
    
    this->top->capture_newframe = 1;
    
    uint8_t b = (px[0] >> 4) & 0xF;
    uint8_t g = (px[1] >> 4) & 0xF;
    uint8_t r = (px[2] >> 4) & 0xF;
    this->top->datain = (r << 8) | (g << 4) | b;

    this->top->wRgbfilter = *(this->wRgbfilter);

    this->input_addr++;
    if (this->input_addr >=
        ((*this->input_image)->rows * (*this->input_image)->cols)) {
      this->input_addr = 0;
    }
  }

 private:
  Vdesign_top *const top;
  const uint8_t *wRgbfilter;
  const cv::Mat **input_image;
  size_t input_addr;
};

class OutputMonitor : public SimElement {
 public:
  OutputMonitor(Vdesign_top *top, cv::Mat &output_image)
      : top{top}, output_image{output_image}, output_addr{0} {}

  virtual ~OutputMonitor() {}

  virtual void onReset() { this->output_addr = 0; }

  virtual void preCycle() { this->top->addrout = this->output_addr; }

  virtual void postCycle() {
    auto image_data = this->output_image.ptr<BGRAPixel>();
    auto px = &(image_data[this->output_addr]);

    (*px)[0] = (top->dataout & 0xF) << 4; //b
    (*px)[1] = ((top->dataout >> 4) & 0xF) << 4; //g
    (*px)[2] = ((top->dataout >> 8) & 0xF) << 4; //r
    (*px)[3] = ALPHA_SOLID; //alpha

    this->output_addr++;
    if (this->output_addr >=
        (this->output_image.rows * this->output_image.cols)) {
      this->output_addr = 0;
    }


  }

 private:
  Vdesign_top *const top;
  cv::Mat output_image;
  size_t output_addr;
};

Vdesign_top *initDut(int argc, char **argv) {
  Verilated::randReset(2);  // Randomize all bits
  Verilated::traceEverOn(true);
  Verilated::commandArgs(argc, argv);
  Vdesign_top *top = new Vdesign_top;

  return top;
}

void deinitDut(Vdesign_top **top, VerilatedVcdC *m_trace) {
  if (m_trace != 0) {
    m_trace->flush();
    m_trace->close();
    delete m_trace;
  }
  (*top)->final();
  delete *top;
  *top = 0;
}

VerilatedVcdC *initTrace(Vdesign_top *top) {
  VerilatedVcdC *m_trace = new VerilatedVcdC;
  top->trace(m_trace, 99);
  m_trace->open("sim.vcd");
  return m_trace;
}

void dumpTrace(VerilatedVcdC *m_trace, vluint64_t *sim_time,
               bool flush = false) {
  if (m_trace != 0) {
    m_trace->dump(*sim_time);
    if (flush) m_trace->flush();
  }
  *sim_time += 1;
}

void tickDut(Vdesign_top *top, const std::vector<SimElement *> &sim_elements,
             vluint64_t *sim_time, VerilatedVcdC *m_trace = 0) {
  top->clk = 0;
  top->eval();

  for (SimElement *e : sim_elements) e->preCycle();
  top->clk = 1;
  top->eval();
  for (SimElement *e : sim_elements) e->postCycle();
  dumpTrace(m_trace, sim_time);

  top->clk = 0;
  top->eval();
  dumpTrace(m_trace, sim_time, true);
}

  //8 bit
  static double PosToW_inner(unsigned char pos){
 

   std::bitset<8> bitset{pos};
 
   std::cout << "Pos [" <<  bitset << "]"<<std::endl;

   int count=0;  
   //default angular movement  
   double w=0.05;

   for (int i = 8; i > 4; i--) {
        int led_n = i - 1;
        bool led_on = pos & (1 << led_n);
	if (led_on) count--;
   }

   for (int i = 4; i > 0; i--) {
        int led_n = i - 1;
        bool led_on = pos & (1 << led_n);
	if (led_on) count++;
   }
 

   std::cout << "C [" <<  count << "]"<<std::endl;
	//return count>0 ? 0.15:-0.15;
	
  if (count>0){
    w=0.15;
  }else if(count<0){
    w=-0.15;
  }else{
    w=0.1;
  }
  
  return w;
  }

  //3 bit
  static double DisToV_inner(unsigned char dis){
  
    std::cout << "inner "<<std::endl;
	  
    std::bitset<3> bitset{dis};

    unsigned long dist = bitset.to_ulong();
    double v = 0.0;

    std::cout << "Dis [" <<  bitset << "]: "<< dist <<std::endl;
   
  switch(dist) {
  case 0:
    break;
  case 1:
    v=0.25;
    break;
  case 2:
    v=0.2;
    break;
  case 3:
    v=0.15;
    break;
  case 4:
    v=0.1;
    break;
  case 5:
    v=0.05;
    break;
  case 6:
    v=0.00;
    break;
  case 7:
    v=-0.05;
    break;
  default:
    v=0.00;
}
  return v;
  }

  void publishVW(double v, double w, ros::Publisher &pub){

   geometry_msgs::Twist base_cmd;

   base_cmd.linear.x = base_cmd.linear.y = base_cmd.angular.z = 0;

   base_cmd.linear.x = v;
   base_cmd.angular.z = w;

   pub.publish(base_cmd);

  }


void resetDut(Vdesign_top *top, const std::vector<SimElement *> &sim_elements,
              vluint64_t *sim_time, VerilatedVcdC *m_trace = 0,
              int reset_cycles = 10) {
  top->rst = 1;
  for (int i = 0; i < reset_cycles; i++) {
    tickDut(top, sim_elements, sim_time, m_trace);
  }
  top->rst = 0;

  for (SimElement *e : sim_elements) e->onReset();
}


cv::Mat input_feed;

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {

     input_feed = cv_bridge::toCvCopy(msg, "bgr8")->image;

  //    cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
   cv::waitKey(30);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}


// Main code
int main(int argc, char **argv) {
  // init buffers

  const cv::Mat input_image_1 = cv::imread(cv::String{input_image_1_path});
  assert(input_image_1.channels() == 3 && input_image_1.cols == cols &&
         input_image_1.rows == rows && input_image_1.isContinuous());

 /* Enable WEBCAM Feed*/
/**/
  cv::VideoCapture cap(0);

  if (!cap.isOpened()) {

          std::cout << "cannot open camera";
  }

  //Init Video Input 
  cv::Mat resized_input_feed;
  
  input_feed = input_image_1;
  cv::Mat output_image(rows, cols, CV_8UC4);


//  uint8_t wRgbfilter = 0x00;  // no filter
  uint8_t wRgbfilter = 0x01;  // red?

  // init dut, tracing and sim elements
  Vdesign_top *top = initDut(argc, argv);
  VerilatedVcdC *m_trace = initTrace(top);

  const cv::Mat *input_image = &input_image_1;

  std::vector<SimElement *> simElements;
  simElements.push_back(new InputDriver(top, &wRgbfilter, &input_image));
  simElements.push_back(new OutputMonitor(top, output_image));

  vluint64_t sim_time = 0;
  resetDut(top, simElements, &sim_time, m_trace);

  // Our state
  bool done = false;
  bool show_demo_window = false;
  bool running = false;
  bool do_reset = false;
  int step_n_cycles = 0;
  int cycles_per_iteration = 500;
  

  //ROS Integration
  /*
  ros::init(argc, argv, "image_listener");

  // /image_raw
  ros::NodeHandle image_nh;

  image_transport::ImageTransport it(image_nh);
  image_transport::Subscriber sub = it.subscribe("image_raw", 1, imageCallback);

  // /cmd_vel
  ros::NodeHandle motor_nh;

  ros::Publisher cmd_vel_pub_;
  cmd_vel_pub_ = motor_nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
  


*/

  // Main loop
  while (!done) {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
    // tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to
    // your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
    // data to your main application. Generally you may always pass all inputs
    // to dear imgui, and hide them from your application based on those two
    // flags.
    //





    //ROS Callback handling attached to the main loop
    //ros::spinOnce();

    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    // main window
        step_n_cycles = cycles_per_iteration;


//WEBCAM ENABLE
      cap >> input_feed;

//
      cv::resize(input_feed,resized_input_feed,cv::Size(cols,rows),cv::INTER_LINEAR);

//      assert(input_feed.channels() == 3 && input_feed.cols == cols &&
//         input_feed.rows == rows && input_feed.isContinuous());

      input_image = &resized_input_feed;

      static double w = PosToW_inner(top->leds);

      static double v = DisToV_inner(top->proximity);
         std::bitset<3> bitset{top->proximity};

    unsigned long dist = bitset.to_ulong();
   
    std::cout << "Dis [" <<  bitset << "]: "<< dist <<std::endl; 
   std::cout << " vuelta"<<std::endl;

      //ROS Publish
//      publishVW(v,w,cmd_vel_pub_);
   


    // dut eval
    if (do_reset) {
      resetDut(top, simElements, &sim_time, m_trace);
    }

    while (!Verilated::gotFinish() && step_n_cycles > 0) {
      step_n_cycles--;
      tickDut(top, simElements, &sim_time, m_trace);
    }


  }

  // Cleanup
  return 0;
}
