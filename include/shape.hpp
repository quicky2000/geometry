/*
  This file is part of geometry
  Copyright (C) 2015  Julien Thevenon ( julien_thevenon at yahoo.fr )

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

#ifndef _SHAPE_HPP_
#define _SHAPE_HPP_

#include "point.hpp"
#include "segment.hpp"
#include <vector>
#include <cinttypes>
#include <limits>

namespace geometry
{
  class shape
  {
  public:
    inline shape(void);
    inline uint32_t get_nb_point(void)const;
    inline uint32_t get_nb_segment(void)const;
    inline const segment & get_segment(const uint32_t & p_index)const;
    inline const point & get_point(const uint32_t & p_index)const;
    inline virtual bool contains(const point & p)const=0;
    inline virtual ~shape(void){}
  protected:
    inline void internal_add(const point & p_point);
    inline void internal_add(const segment & p_segment);
    inline void remove_last_segment(void);
  private:
    std::vector<point> m_points;
    std::vector<segment> m_segments;
    double m_min_x;
    double m_max_x;
    double m_min_y;
    double m_max_y;
  };

  //------------------------------------------------------------------------------
  shape::shape(void):
    m_min_x(std::numeric_limits<double>::max()),
    m_max_x(std::numeric_limits<double>::lowest()),
    m_min_y(std::numeric_limits<double>::max()),
    m_max_y(std::numeric_limits<double>::lowest())
  {
  }

  //------------------------------------------------------------------------------
  uint32_t shape::get_nb_point(void)const
  {
    return m_points.size();
  }

  //------------------------------------------------------------------------------
  uint32_t shape::get_nb_segment(void)const
  {
    return m_segments.size();
  }

  //------------------------------------------------------------------------------
  const segment & shape::get_segment(const uint32_t & p_index)const
  {
    assert(p_index < m_segments.size());
    return m_segments[p_index];
  }

  //------------------------------------------------------------------------------
  const point & shape::get_point(const uint32_t & p_index)const
  {
    assert(p_index < m_points.size());
    return m_points[p_index];
  }

  //------------------------------------------------------------------------------
  void shape::internal_add(const point & p_point)
  {
    if(p_point.get_x() > m_max_x) m_max_x = p_point.get_x();
    if(p_point.get_y() > m_max_y) m_max_y = p_point.get_y();
    if(p_point.get_x() < m_min_x) m_min_x = p_point.get_x();
    if(p_point.get_y() < m_min_y) m_min_y = p_point.get_y();
    m_points.push_back(p_point);
  }

  //------------------------------------------------------------------------------
  void shape::internal_add(const segment & p_segment)
  {
    m_segments.push_back(p_segment);
  }

  //------------------------------------------------------------------------------
  void shape::remove_last_segment(void)
  {
    assert(m_segments.size());
    m_segments.pop_back();
  }

  //------------------------------------------------------------------------------
  bool shape::contains(const point & p)const
  {
    if(m_min_x <= p.get_x() && p.get_x() <= m_max_x && m_min_y <= p.get_y() && p.get_y() <= m_max_y)
      {
	return true;
      }
    else
      {
	return false;
      }
  }
}
#endif // _SHAPE_HPP_
//EOF
