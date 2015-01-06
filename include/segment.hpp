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
#ifndef _SEGMENT_HPP_
#define _SEGMENT_HPP_

#include "point.hpp"
#include "assert.h"

class segment
{
public:
  inline segment(const point & p_source, const point & dest);
  inline const point & get_source(void)const;
  inline const point & get_dest(void)const;
  inline bool is_horizontal(void)const;
  inline bool is_vertical(void)const;
  inline double get_x(const double & p_y)const;
  inline double get_y(const double & p_x)const;
  inline bool belong(const point & p_point)const;
  inline double get_side(const point & p_point)const;
  inline double vectorial_product(const segment & p_seg)const;
  inline double scalar_product(const segment & p_seg)const;
  inline double get_square_size(void)const;
  inline static bool check_convex_continuation(const double & p_vec_prod,double & p_orient);
private:
  point m_source;
  point m_dest;
  double m_coef_x;
  double m_coef_y;
  bool m_horizontal;
  bool m_vertical;
  double m_min_x;
  double m_max_x;
  double m_min_y;
  double m_max_y;
};

segment::segment(const point & p_source, const point & p_dest):
    m_source(p_source),
    m_dest(p_dest),
    m_coef_x(p_dest.get_x() - p_source.get_x()),
    m_coef_y(p_dest.get_y() - p_source.get_y()),
    m_horizontal(m_coef_x==0),
    m_vertical(m_coef_y==0),
    m_min_x( p_source.get_x() <= p_dest.get_x() ? p_source.get_x() : p_dest.get_x()),
    m_max_x( p_source.get_x() >= p_dest.get_x() ? p_source.get_x() : p_dest.get_x()),
    m_min_y( p_source.get_y() <= p_dest.get_y() ? p_source.get_y() : p_dest.get_y()),
    m_max_y( p_source.get_y() >= p_dest.get_y() ? p_source.get_y() : p_dest.get_y())
{
}

const point & segment::get_source(void)const
{
  return m_source;
}

const point & segment::get_dest(void)const
{
  return m_dest;
}

bool segment::is_horizontal(void)const
{
  return m_horizontal;
}

bool segment::is_vertical(void)const
{
  return m_vertical;
}

double segment::get_x(const double & p_y)const
{
  assert(!m_horizontal);
  double l_x = m_source.get_x();
  if(!m_vertical)
    {
      double l_t = (p_y - m_source.get_y())/(m_dest.get_y() - m_source.get_y());
      l_x = (m_dest.get_x() - m_source.get_x()) * l_t + m_source.get_x();
    }
  return l_x;
}

double segment::get_y(const double & p_x)const
{
  assert(!m_vertical);
  double l_y = m_source.get_y();
  if(!m_horizontal)
    {
      double l_t = (p_x - m_source.get_x())/(m_dest.get_x() - m_source.get_x());
      l_y = (m_dest.get_y() - m_source.get_y()) * l_t + m_source.get_y();
    }
  return l_y;
}

bool segment::belong(const point & p_point)const
{
  return p_point.get_y() == this->get_y(p_point.get_x()) && p_point.get_x() >= m_min_x && p_point.get_x() <= m_max_x && p_point.get_y() >= m_min_y && p_point.get_y() <= m_max_y;
}

double segment::get_side(const point & p_point)const
{
  return m_coef_x * (p_point.get_y() - m_source.get_y()) - m_coef_y * (p_point.get_x() - m_source.get_x());
}

double segment::vectorial_product(const segment & p_seg)const
{
  return m_coef_x * p_seg.m_coef_y - m_coef_y * p_seg.m_coef_x ;
}

double segment::scalar_product(const segment & p_seg)const
{
  return m_coef_x * p_seg.m_coef_x + m_coef_y * p_seg.m_coef_y ;
}

double segment::get_square_size(void)const
{
  return m_coef_x * m_coef_x+ m_coef_y * m_coef_y;
}

bool segment::check_convex_continuation(const double & p_vec_prod,double & p_orient)
{
  bool l_convex = true;
  if(p_orient < 0)
    {
      l_convex = p_vec_prod <= 0;
    }
  else if(p_orient > 0)
    {
      l_convex = p_vec_prod >= 0;
    }
  else
    {
      p_orient = p_vec_prod;
    }
  return l_convex;
}

#endif /* _SEGMENT_HPP_ */
