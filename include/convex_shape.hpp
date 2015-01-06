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
  double m_vectorial_product;
};

//------------------------------------------------------------------------------
convex_shape::convex_shape(const point & p1,const point & p2,const point & p3):
  m_vectorial_product(0)
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
  m_vectorial_product = m_segments[0].vectorial_product(m_segments[1]);
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
  // Create temporary segment between latest point and new point
  segment l_tmp_segment1(m_points[m_points.size()-1],p);
  segment l_tmp_segment2(p,m_points[0]);

  // Insure that sign of previous vectorial product is kept
  double l_prod_vec1 = m_segments[m_segments.size()-1].vectorial_product(l_tmp_segment1);
  double l_prod_vec2 = l_tmp_segment1.vectorial_product(l_tmp_segment2);
  double l_prod_vec3 = l_tmp_segment2.vectorial_product(m_segments[0]);
  bool l_ok = segment::check_convex_continuation(l_prod_vec1,m_vectorial_product) && segment::check_convex_continuation(l_prod_vec2,m_vectorial_product) && segment::check_convex_continuation(l_prod_vec3,m_vectorial_product);

  // If shape is still convex than add the new point and the related segments
  if(l_ok)
    {
      m_segments.pop_back();
      m_segments.push_back(l_tmp_segment1);
      m_segments.push_back(l_tmp_segment2);
      m_points.push_back(p);
    }

  return l_ok;
}
#endif // _CONVEX_SHAPE_HPP_
