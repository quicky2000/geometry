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
#ifndef _POINT_HPP_
#define _POINT_HPP_

class point
{
public:
  inline point(const double & p_x,const double & p_y);
  inline const double & get_x(void)const;
  inline const double & get_y(void)const;
  inline bool operator<(const point & p2)const; 
private:
  double m_x;
  double m_y;
};


point::point(const double & p_x,const double & p_y):
  m_x(p_x),
  m_y(p_y)
{
}

const double & point::get_x(void)const
{
  return m_x;
}

const double & point::get_y(void)const
{
  return m_y;
}

bool point::operator<(const point & p2)const
{
  return ( m_x != p2.m_x ? m_x < p2.m_x : m_y < p2.m_y);
}

#endif /* _POINT_HPP_ */
