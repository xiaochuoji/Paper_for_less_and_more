#include <iostream>
#include <vector>
#include <string>
#include <cmath>

class CreatFigure{
 public:
    CreatFigure(const std::string &file_path){
        ReadTheFeaturePoints(file_path);
        source_=0;
        sink_=std::pow(2,10);
        second_link_capacity_=1;
        std::cout<<"source is "<< source_<<std::endl;
        std::cout<<"sink is "<<sink_<<std::endl;
    }
    void ReadTheFeaturePoints(const std::string &file_path);
    std::vector<int64_t> CreatFirstNodes();
    std::vector<int64_t>  CreatSecondNodes();
    void CreatThirdNodes(const std::vector<int64_t> &second_points);
    void CalculateCost(const int max_value, const int target_value);
    void CalculateCapacity(const std::vector<int64_t> &first_points ,
                                                   const std::vector<int64_t> &second_points);
    void SetMaxFlowInLink(const int max_flow);
    void CalculateMaxFlow();
    void ShowNodes();

 private:
 std::vector<std::vector<int64_t>> feature_points_;
 std::vector<int64_t> start_nodes_;
 std::vector<int64_t> end_nodes_;
 std::vector<int64_t> capacities_;
 std::vector<int64_t> cost_;
 int64_t source_;
 int64_t sink_;
 int64_t the_max_flow_in_last_link_;
 int64_t second_link_capacity_;

};