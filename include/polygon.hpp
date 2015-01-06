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
#ifndef _POLYGON_HPP_
#define _POLYGON_HPP_

#include "point.hpp"
#include "segment.hpp"
#include "convex_shape.hpp"
#include <vector>
#include <set>
#include <stdint.h>
#include <iostream>

class polygon
{
public:
  inline polygon(const std::vector<point> & p_points);
  inline bool is_convex(void);
  inline uint32_t get_nb_point(void)const;
  inline uint32_t get_nb_segment(void)const;
  inline const segment & get_segment(uint32_t p_index)const;
  inline void cut_in_convex_polygon(void)const;
private:
  std::vector<point> m_points;
  std::vector<segment> m_segments;
  std::set<point> m_convex_wrapping_points;
};

//------------------------------------------------------------------------------
inline uint32_t polygon::get_nb_point(void)const
{
  return m_points.size()-1;
}

//------------------------------------------------------------------------------
inline uint32_t polygon::get_nb_segment(void)const
{
  return m_segments.size();
}

//------------------------------------------------------------------------------
inline const segment & polygon::get_segment(uint32_t p_index)const
{
  assert(p_index < m_segments.size());
  return m_segments[p_index];
}

//------------------------------------------------------------------------------
inline polygon::polygon(const std::vector<point> & p_points)
{
  assert(p_points.size()>=3);
  std::vector<point>::const_iterator l_iter_point = p_points.begin();
  std::vector<point>::const_iterator l_iter_point_end = p_points.end();
  std::cout << "List of point : " << std::endl ;
  while(l_iter_point != l_iter_point_end)
    {
      std::cout << "(" << l_iter_point->get_x() << "," << l_iter_point->get_y() << ")" << std::endl ;
      ++l_iter_point;
    }

  // Search for "minimum point"
  point l_min = p_points[0];
  uint32_t l_min_index = 0;
  uint32_t l_index = 0;
  for(l_index = 1; l_index < p_points.size();++l_index)
    {
      std::cout << "Checking index " << l_index << std::endl ;
      if(p_points[l_index] < l_min)
	{
	  l_min_index = l_index;
	  l_min = p_points[l_index];
	  std::cout << "New min found at index " << l_index << " : (" << l_min.get_x() << " , " << l_min.get_y() << ")" << std::endl ;
	}
    }
  l_index = l_min_index;

  // Fill point list from the minimum point and duplicate it at the end of list
  while(m_points.size() <= p_points.size())
    {
      m_points.push_back(p_points[l_index]);
      l_index = (l_index + 1) % p_points.size();
    }
  l_iter_point = m_points.begin();
  l_iter_point_end = m_points.end();
  std::cout << "List of point : " << std::endl ;
  while(l_iter_point != l_iter_point_end)
    {
      std::cout << "(" << l_iter_point->get_x() << "," << l_iter_point->get_y() << ")" << std::endl ;
      ++l_iter_point;
    }

  // Create list of segments
  for(l_index = 1 ; l_index < m_points.size();++l_index)
    {
      m_segments.push_back(segment(m_points[l_index-1],m_points[l_index]));
    }
  std::vector<segment>::const_iterator l_iter = m_segments.begin();
  std::vector<segment>::const_iterator l_iter_end = m_segments.end();
  std::cout << "List of segment" << std::endl ;
  while(l_iter != l_iter_end)
    {
      std::cout << "(" << l_iter->get_source().get_x() << "," ;
      std::cout << l_iter->get_source().get_y() << ") -> " ;
      std::cout << "(" << l_iter->get_dest().get_x() << "," ;
      std::cout << l_iter->get_dest().get_y() << ")" << std::endl ;
      ++l_iter;
    }
  //  m_segments.push_back(segment(m_points[l_index],m_points[0]));
  
}

bool polygon::is_convex(void)
{
  m_convex_wrapping_points.clear();
  std::vector<point> l_convex_wrapping;
  // First point is minimum so it is mandatory included in convex wrapping
  l_convex_wrapping.push_back(m_points[0]);
  uint32_t l_current_src_index = 0;

  // Check immediate following point that wrap all other points
  uint32_t l_current_dest_index = (l_current_src_index + 1) % (m_points.size() - 1);

  do
    {
      bool l_convex = true;
      do
	{
	  std::cout << "l_current_src_index = " << l_current_src_index << std::endl ;
	  std::cout << "l_current_dest_index = " << l_current_dest_index << std::endl ;
	  std::cout << "Check if point(" << m_points[l_current_dest_index].get_x() << "," << m_points[l_current_dest_index].get_y() << ") belongs to convex wrapping" << std::endl ;
	  // Create temp segment
	  segment l_tmp_ref_seg(m_points[l_current_src_index],m_points[l_current_dest_index]);
	  double l_orient = 0;
	  l_convex = true;
	  for(uint32_t l_index = 1 ;l_index < m_points.size() && l_convex; ++l_index)
	    {
	      std::cout << "l_index = " << l_index << std::endl ;
	      if(l_index != l_current_dest_index && l_index != l_current_src_index)
		{
		  std::cout << "Check point( " << m_points[l_index].get_x() << "," << m_points[l_index].get_y() << ")" <<  " position compared to ref segment" << std::endl ;
		  l_convex = segment::check_convex_continuation(l_tmp_ref_seg.vectorial_product(segment(m_points[l_current_src_index],m_points[l_index])),l_orient);
		}
	    }
	  if(!l_convex)
	    {
	      ++l_current_dest_index;
	      assert(l_current_dest_index < m_points.size()-1);
	    }
	}
      while(!l_convex);
      std::cout << "point(" << m_points[l_current_dest_index].get_x() << "," << m_points[l_current_dest_index].get_y() << ") belongs to convex wrapping" << std::endl ;
      l_convex_wrapping.push_back(m_points[l_current_dest_index]);
      m_convex_wrapping_points.insert(m_points[l_current_dest_index]);
      l_current_src_index = l_current_dest_index;
      l_current_dest_index = (l_current_src_index + 1) % (m_points.size() -1);
    }
  while(l_current_dest_index);

  //  double l_orient = m_segments[m_segments.size()-1].vectorial_product(m_segments[0]);
  //  uint32_t l_index = 0;
  //  while(l_convex && l_index + 1 < m_segments.size())
  //    {
  //      l_convex = check_convex_continuation(m_segments[l_index].vectorial_product(m_segments[l_index+1]),l_orient);
  //      ++l_index;
  //    }

  return l_convex_wrapping.size() == m_points.size()-1;
}

#include <deque>

void polygon::cut_in_convex_polygon(void)const
{
  //TO DELETE  std::deque<point> l_convex_shape;
  //TO DELETE  std::set<point> l_convex_shape_points;
  //TO DELETE  std::vector<segment> l_convex_shape_segments;
  //TO DELETE  while(l_convex_shape.size() < 3)
  //TO DELETE    {
      bool l_valid_starting_shape = false;
      for(uint32_t l_index = 0 ; l_index < m_points.size()-1 && !l_valid_starting_shape; ++l_index)
	{
	  std::cout << "Index = " << l_index << std::endl ;
	  if(m_convex_wrapping_points.find(m_points[(l_index+1) % (m_points.size()-1)]) != m_convex_wrapping_points.end())
	    {
	      std::cout << "Condition validated" << std::endl ;
	      convex_shape l_convex_shape(m_points[(l_index) % (m_points.size()-1)],m_points[(l_index+1) % (m_points.size()-1)],m_points[(l_index+2) % (m_points.size()-1)]);
											    
	      //TO DELETE	      l_convex_shape.clear();
	      //TO DELETE	      	      l_convex_shape.push_back(m_points[(l_index) % (m_points.size()-1)]);
	      //TO DELETE	      	      l_convex_shape.push_back(m_points[(l_index+1) % (m_points.size()-1)]);
	      //TO DELETE	      	      l_convex_shape.push_back(m_points[(l_index+2) % (m_points.size()-1)]);
	      //TO DELETE	      	      l_convex_shape_points.clear();
	      //TO DELETE	      	      l_convex_shape_points.insert(m_points[(l_index) % (m_points.size()-1)]);
	      //TO DELETE	      	      l_convex_shape_points.insert(m_points[(l_index+1) % (m_points.size()-1)]);
	      //TO DELETE	      	      l_convex_shape_points.insert(m_points[(l_index+2) % (m_points.size()-1)]);
	      //TO DELETE	      	      l_convex_shape_segments.clear();
	      //TO DELETE	      	      l_convex_shape_segments.push_back(segment(m_points[(l_index) % (m_points.size()-1)],m_points[(l_index+1) % (m_points.size()-1)]));
	      //TO DELETE	      	      l_convex_shape_segments.push_back(segment(m_points[(l_index+1) % (m_points.size()-1)],m_points[(l_index+2) % (m_points.size()-1)]));
	      //TO DELETE	      	      l_convex_shape_segments.push_back(segment(m_points[(l_index+2) % (m_points.size()-1)],m_points[(l_index) % (m_points.size()-1)]));

	      // Display shape points
	      l_convex_shape.display_points();
	      //TO DELETE	      for(uint32_t l_index_shape_point = 0 ; l_index_shape_point < l_convex_shape.size();++l_index_shape_point)
	      //TO DELETE		{
	      //TO DELETE		  std::cout << "Shape point(" << l_convex_shape[l_index_shape_point].get_x() << "," << l_convex_shape[l_index_shape_point].get_y() << ")" << std::endl ;
	      //TO DELETE		}

	      // Check that there are no polygon summit inside shape
	      std::vector<point>::const_iterator l_iter = m_points.begin();
	      std::vector<point>::const_iterator l_iter_end = m_points.end();
	      while(l_iter != l_iter_end)
		{
		  if(!l_convex_shape.find(*l_iter))
		    {
		      std::cout << "Point(" << l_iter->get_x() << "," << l_iter->get_y() << ") is not part of convex_shape" << std::endl ;
		      if(l_convex_shape.contains(*l_iter))
			{
			  std::cout << "NOK : in shape" << std::endl ;
			}
		      else
			{
			  std::cout << "OK : Not in shape" << std::endl ;
			}
		    }
		  ++l_iter;
		}
	    }
	}
   //TO DELETE   }
}
#endif /* _POLYGON_HPP_ */
