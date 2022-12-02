#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include<cstdint>

#include <Eigen/Core>
#include "creatfigure.h"
#include "ortools/graph/max_flow.h"

void CreatFigure::ReadTheFeaturePoints(const std::string &file_path){
    std::ifstream in_files(file_path);
    if (!in_files) {
        std::cout<<"there is no file"<<std::endl;
        std::cout<<"creat class CreatFigure failed"<<std::endl;
        return ;
    }
    while(!in_files.eof()){
        std::vector<int64_t> feature;
        std::string str;
        std::getline(in_files,str);
        std::string tepmorary_str;
        std:: stringstream theline(str);
        while(theline >> tepmorary_str){
            auto g=atoi(tepmorary_str.c_str());
            static_cast<int64_t>(g);
             feature.emplace_back(g);
        }
        feature_points_.emplace_back(feature);     
    }
}

std::vector<int64_t> CreatFigure::CreatFirstNodes(){
    bool if_repeat=false;
    bool if_first=true;
    for (auto &key_fream:feature_points_) {
        for (auto &key_point:key_fream){
            if (if_first){
                end_nodes_.emplace_back(key_point);
                start_nodes_.emplace_back(source_);
            } else {
                 std::vector<int64_t> ::iterator if_find_repeat=end_nodes_.begin();
                 if_find_repeat=find(end_nodes_.begin(),end_nodes_.end(),key_point);
                 if (if_find_repeat==end_nodes_.end()){
                    end_nodes_.emplace_back(key_point);
                    start_nodes_.emplace_back(source_);
                 }
            }
        }
        if_first=false;
    }
    std::vector<int64_t> first_end_nodes=end_nodes_;
    return first_end_nodes;
}

std::vector<int64_t> CreatFigure::CreatSecondNodes() {
        auto the_max_number_in_first_ptr=std::max_element(end_nodes_.begin(),end_nodes_.end());
        int64_t the_new_max=*the_max_number_in_first_ptr+1;
        int number_of_fream=feature_points_.size();
        std::vector<int64_t> second_nodes_tep;
        std::vector<int64_t> copy_first_end_nodes=end_nodes_;
        for (auto &sec_point: copy_first_end_nodes) {
            Eigen::VectorXi locate=Eigen::VectorXi::Zero(number_of_fream);
            // std::cout<<"the sec_point is "<<sec_point<<std::endl;
            int cout=0;
            for (auto &fream_point : feature_points_) {
                std::vector<int64_t>::iterator locate_ptr=fream_point.end();
                locate_ptr=find(fream_point.begin(),fream_point.end(),sec_point);
                if (locate_ptr!=fream_point.end()) {
                locate[cout]=1;
                }
                ++cout;
            }
            // std::cout<<"the locate is \n"<<locate.transpose()<<std::endl;
            for (int i=0 ; i<number_of_fream-1 ; ++i) {
                for (int j=i+1 ; j<number_of_fream ; ++j){
                    if (locate[i]==1 && locate[j]==1){
                        int64_t number_of_this_nodes=the_new_max-1+i*number_of_fream-i*(i+1)/2+j-i;
                        start_nodes_.emplace_back(sec_point);
                        end_nodes_.emplace_back(number_of_this_nodes);
                        second_nodes_tep.emplace_back(number_of_this_nodes);
                    }
                }
            }
        }
    return second_nodes_tep;
}

void CreatFigure::CreatThirdNodes(const std::vector<int64_t> &second_points){
    std::vector<int64_t> third_nodes;
    bool flag=true;
    for (auto &point:second_points){
        if (flag){
            third_nodes.emplace_back(point);
            start_nodes_.emplace_back(point);
            end_nodes_.emplace_back(sink_);
            flag=false;
        } else{
            auto if_in=find(third_nodes.begin(),third_nodes.end(),point);
            if (if_in!=third_nodes.end()){
                continue;
            }
            third_nodes.emplace_back(point);
            start_nodes_.emplace_back(point);
            end_nodes_.emplace_back(sink_);
        }
    }
}

void CreatFigure::CalculateCapacity(const std::vector<int64_t> &first_points,
                                                                         const std::vector<int64_t> &second_points){
    for (auto &points:first_points){
        int cout=0;
        for (auto &fream:feature_points_){
            std::vector<int64_t>::iterator find_ptr=fream.end();
            find_ptr=find(fream.begin(),fream.end(),points);
            if (find_ptr!=fream.end()) {
                ++cout;
            }
        }
        int the_capacity=cout*(cout-1)/2;
        capacities_.emplace_back(the_capacity);
    }
    for (int  i=0; i<second_points.size(); ++i){
        capacities_.emplace_back(second_link_capacity_);
    }
    for (int  i=0; i<second_points.size(); ++i){
        capacities_.emplace_back(the_max_flow_in_last_link_);
    }
}

void CreatFigure::SetMaxFlowInLink(const int max_flow){
    the_max_flow_in_last_link_=max_flow;
    std::cout<<"the max flow set on the last link is:    "<<the_max_flow_in_last_link_<<std::endl;
    }

void CreatFigure::CalculateMaxFlow(){
    operations_research::SimpleMaxFlow max_flow;
    for (int i=0;i<start_nodes_.size();++i) {
        max_flow.AddArcWithCapacity(start_nodes_[i],end_nodes_[i],capacities_[i]);
    }
    int status=max_flow.Solve(source_,sink_);
    if (status==operations_research::MaxFlow::OPTIMAL) {
        std::cout<<"the max flow is:  "<<max_flow.OptimalFlow();
    } else {
        std::cout<<"solving failed \n";
        std::cout<<"get:   "<<max_flow.OptimalFlow()<<std::endl;
    }
 }

void CreatFigure::CalculateCost(const int max_value, const int target_value){
    
}

void CreatFigure::ShowNodes(){
    for(auto &fream:feature_points_){
        std::cout<<"the  feature point is ";
        for (auto &points:fream){
        std::cout<<points;
        }
        std::cout<<"\n";
    }

    std::cout<<"the start nodes is "<<std::endl;
    for (auto &start:start_nodes_){
        std::cout<< start<<"   ";
    }
    std::cout<<"\n the end nodes is "<<std::endl;
     for (auto &end:end_nodes_){
        std::cout<< end<<"   ";
    }
    std::cout<<" \n the capacity nodes is:   "<<std::endl;
    for (auto &capacity:capacities_){
        std::cout<<capacity<<"   ";
    }
}
