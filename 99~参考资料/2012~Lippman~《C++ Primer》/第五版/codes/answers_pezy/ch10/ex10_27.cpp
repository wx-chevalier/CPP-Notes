//
//  ex10_27.cpp
//  Exercise 10.27
//
//  Created by pezy on 12/13/14.
//  Copyright (c) 2014 pezy. All rights reserved.
//
//  In addition to unique, the library defines function named unique_copy that
//  takes a third iterator denoting a destination into which to copy the unique
//  elements.
//  Write a program that uses unique_copy to copy the unique elements from
//  a vector into an initially empty list.

#include <iostream>
#include <algorithm>
#include <vector>
#include <list>

int main()
{
    std::vector<int> vec{3, 5, 1, 5, 1, 7, 3, 7, 9};
    std::list<int> lst;

    std::sort(vec.begin(), vec.end());
    std::unique_copy(vec.begin(), vec.end(), back_inserter(lst));
    for (auto i : lst) std::cout << i << " ";
    std::cout << std::endl;
}
