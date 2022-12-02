#include <iostream>
#include <vector>
#include <string>

#include "creatfigure.h"




int main(int argc,char **argv){
    std::string file_path="../test_point.txt";
    CreatFigure test(file_path);
    int64_t max_flow=2;
    test.SetMaxFlowInLink(max_flow);
    std::vector<int64_t> first_end_point=test.CreatFirstNodes();
    std::vector<int64_t> second_end_point=test.CreatSecondNodes();
    test.CreatThirdNodes(second_end_point);
    test.CalculateCapacity(first_end_point,second_end_point);
    test.CalculateMaxFlow();
    test.ShowNodes();
    std::cout<<"\n maybe success"<<std::endl;
    return 0;

}