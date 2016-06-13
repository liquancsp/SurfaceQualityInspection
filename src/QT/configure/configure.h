#ifndef CONFIGURE_H
#define CONFIGURE_H

namespace lq {
  //use in main_panel.cpp
  const char TREE_TITLE[] = "Model List";
  const char MODEL[] = "model";
  const int STD_MAX_RANGE = 100;
  const int STD_ZERO = 0;
  const int STD_DEFAULT_VALUE = 50;
  //use in model_win.cpp
  const char ABOUT[] = "About";
  const char ABOUT_CONTENT[] =
      "This is surface quality inspection program\nversion 1.0\n";
  const char  WARNNING_TITLE[] = "warnning";
  const char  NO_WIN_DISPLAY[] = "this is no window to display file.";
  const char  FAIL_OPEN_DATA[] = "Fail to open local align data(lad).";
  const char  FAIL_INITIALIZE_DOC[] = "Fail to init QDomDocument.";
  const char  WRONG_MATCH[] = "The data don't match the two model.";
  const char  NO_EXPORT_DATA[] = "There is no data to export.";
  const char  NO_TWO_MODEL[] =
      "there is no two model to compare!\nplease load two model first!";
  const char  NO_COMPARE_TWO_MODEL[] = "You should compute error first!";
  const char  NO_COLOR_FILE[] = "Can't find color data!";
  const char  WINDOWS_INIT_DONE[] = "System ready!";
  const char  SUCCESS_LOAD_LAB[] = "Load local align data successful...";
  const char  LOAD_MODEL_FILE_DONE[] = "Load model file successful...";
  const char  COMPLETE_ALGORITHM[30][60] = {"Complete Reflection Map algorithm...",
                                            "Complete Integral Reflection Map algorithm...",
                                            "Complete Global Align Integral Reflection Map Algorithm...",
                                            "Complete Global Align Normal Component Algorithm...",
                                            "Complete Local Align Integral Reflection Map Algorithm...",
                                            "Complete Local Align Normal Component Algorithm...",
                                            "Complete Global Least Squares Integral Reflection Map...",
                                            "Complete Global Least Squares Normal Component Algorithm...",
                                            "Complele Convex Different Algorithm...",
                                            "Complete Mix the Reflection and Convex Algorithm...",
                                            "Complete Mix the Integral and Convex algorithm..",
                                            "Complete Mean Curvature algorithm...",
                                            "Complete Gaussian Curvature algorithm...",
                                            "Complete K1 Principal Curvature algorithm...",
                                            "Complete K2 Principal Curvature algorithm...",
                                            "Complete Normal Vector Angle algorithm...",
                                            "Complete Distance Different algorithm...",
                                            "Complete Mix the Mean Curvature and Integral...",
                                            "Complete Mix the Gaussian Curvature and Integral...",
                                            "Complete Mix the K1 and Integral...",
                                            "Complete Mix the K2 and Integral...",
                                            "Complete Defect Fitting...",
                                            "Complete Zerniker Propagation...",
                                            "Complete Calculate Zerniker Coefficient...",
                                            "Complete Zerniker Classification...",
                                            "Complete Mean Curvature Zerniker Coefficient...",
                                            "Complete Gaussian Curvature Zerniker Coefficient...",
                                            "Complete K1 Curvature Zerniker Coefficient...",
                                            "Complete K2 Curvature Zerniker Coefficient...",
                                            "NULL"};
  const char  SUCCESS_EXPORT_LAB[] = "Export local align data successful...";
  enum
  {
    kChecked = 0,
    kNotChecked
  };
  
}
#endif
