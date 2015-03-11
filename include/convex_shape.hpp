/*
    This file is part of geometry
    Copyright (C) 2011  Julien Thevenon ( julien_thevenon at yahoo.fr )

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#ifndef _CONVEX_SHAPE_HPP_
#define _CONVEX_SHAPE_HPP_

#include "point.hpp"
#include "segment.hpp"
#include <vector>
#include <set>

#include <iostream>

class convex_shape
{
public:
  convex_shape(const point & p1,const point & p2,const point & p3);
  bool find(const point & p)const;
  bool contains(const point & p)const;
  bool add(const point & p);
  void display_points(void)const;
private:
  std::vector<point> m_points;
  std::vector<segment> m_segments;
  std::set<point> m_sorted_points;
};

//------------------------------------------------------------------------------
convex_shape::convex_shape(const point & p1,const point & p2,const point & p3)
{
  m_points.push_back(p1);
  m_points.push_back(p2);
  m_points.push_back(p3);
  m_sorted_points.insert(p1);
  m_sorted_points.insert(p2);
  m_sorted_points.insert(p3);
  m_segments.push_back(segment(p1,p2));
  m_segments.push_back(segment(p2,p3));
  m_segments.push_back(segment(p3,p1));
}

//------------------------------------------------------------------------------
bool convex_shape::find(const point & p)const
{
  return m_sorted_points.find(p) != m_sorted_points.end();
}


//------------------------------------------------------------------------------
bool convex_shape::contains(const point & p)const
{
  std::vector<segment>::const_iterator l_iter = m_segments.begin();
  std::vector<segment>::const_iterator l_iter_end = m_segments.end();
  double l_orient = 0;
  bool l_contain = true;
  while(l_iter != l_iter_end && l_contain)
    {
      l_contain = segment::check_convex_continuation(l_iter->vectorial_product(segment(l_iter->get_source(),p)),l_orient);
      ++l_iter;
    }
  return l_contain;
}

//------------------------------------------------------------------------------
void convex_shape::display_points(void)const
{  
  std::vector<point>::const_iterator l_iter = m_points.begin();
  std::vector<point>::const_iterator l_iter_end = m_points.end();
  while(l_iter != l_iter_end)
    {
      std::cout << "Shape point(" << l_iter->get_x() << "," <<  l_iter->get_y() << ")" << std::endl ;
      ++l_iter;
    }
}

bool convex_shape::add(const point & p)
{
  // For each segment except of latest one that will be broken in 2 segments due to the new point
  bool l_convex = true;
  for(unsigned int l_segment_index = 0 ; l_convex && l_segment_index < m_segments.size() - 1; ++l_segment_index)
    {
      // Get a third point outside of segment
      point l_third_point = m_points[(l_segment_index + 2) & m_points.size()]  ;
      double l_vector_produc = m_segments[l_segment_index].vectorial_product(segment(m_points[l_segment_index],l_third_point));
      l_convex = segment::check_convex_continuation(m_segments[l_segment_index].vectorial_product(segment(m_points[l_segment_index],p)),l_vector_produc);
    }
  if(!l_convex) return false;

  // Create temporary segment between latest point and new point
  segment l_tmp_segment1(m_points[m_points.size()-1],p);
  segment l_tmp_segment2(p,m_points[0]);

  double l_orient = 0;
  for(unsigned int l_point_index = 1 ; l_point_index < m_points.size() - 1;++l_point_index)
    {
      l_convex = segment::check_convex_continuation(l_tmp_segment1.vectorial_product(segment(m_points[m_points.size()-1],m_points[l_point_index])),l_orient);      
    }
  if(!l_convex) return false;
  l_orient = 0;
  for(unsigned int l_point_index = 1 ; l_point_index < m_points.size() - 1;++l_point_index)
    {
      l_convex = segment::check_convex_continuation(l_tmp_segment1.vectorial_product(segment(p,m_points[l_point_index])),l_orient);      
    }
  if(!l_convex) return false;

  m_segments.pop_back();
  m_segments.push_back(l_tmp_segment1);
  m_segments.push_back(l_tmp_segment2);
  m_points.push_back(p);

  return true;
}
#endif // _CONVEX_SHAPE_HPP_
