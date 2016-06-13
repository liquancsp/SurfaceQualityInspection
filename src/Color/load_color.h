#ifndef LOAD_COLOR_H
#define LOAD_COLOR_H
#include <data_type.h>
#include <vector>
#include <fstream>
namespace lq {

  class load_color {

 public:
    load_color();
    ~load_color();
    
    int init_color(std::vector<color> &color_list);

    void generate_error_color(const std::vector<color> &reference_color,
                              const std::vector<double> &tri_error,
                              const int &value,
                              const int &default_value,
                              std::vector<color> &sub_error_color,
                              std::vector<int> &index);

    void generate_error_color(std::vector<color> &reference_color,
                              std::vector<double> &tri_error,
                              const size_t flag, const double scalar,
                              std::vector<int> &index);

    void update_color_list(const std::vector<color> &color_list,
                           const std::vector<color> &sub_error_color,
                           std::vector<color> &change_color_list,
                           const int &value,
                           const int &reference);

    void scale_color_list(double max_error, double min_error,
                          std::vector<color> &color_list);
 private:
    static std::vector<double> init_scalar_;
    double pre_scalar_;
    std::fstream out;
    bool is_scale;

  };
}

#endif
