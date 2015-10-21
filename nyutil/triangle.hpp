/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 Jan Kelling 
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <nyutil/vec.hpp>
#include <nyutil/line.hpp>

namespace nyutil
{

template<size_t dim, typename prec>
class triangle
{
public:
    using value_type = prec;
    using vec_type = vec<dim, prec>;
    using triangle_type = triangle<dim, prec>;
    using line_type = line<dim, prec>;

public:
    vec_type a;
    vec_type b;
    vec_type c;

public:
    triangle() = default;
    triangle(const vec_type& xa, const vec_type& xb, const vec_type& xc) : a(xa), b(xb), c(xc) {}

    ~triangle() = default;

    triangle(const triangle_type& other) = default;
    triangle& operator=(const triangle_type& other) = default;

    triangle(triangle_type&& other) noexcept = default;
    triangle& operator=(triangle_type&& other) noexcept = default;

    //
    inline bool contains(const vec_type& point) const;
    bool intersects(const triangle_type& tri) const { return 0; } //todo
    double size() const { return length(b - a) * length(c - a) * 0.5; }

    //
    float alpha() const { return 0; }
    float beta() const { return 0; }
    float gamma() const { return 0; }

    line_type AB() const { return line_type(a, b); }
    line_type BC() const { return line_type(b, c); }
    line_type AC() const { return line_type(a, c); }

    //conversion
    template<size_t odim, typename oprec>
    operator triangle<odim, oprec>() const { return triangle<odim, oprec>(a, b, c); }
};

//util
template<size_t dim, typename prec>
bool triangle<dim, prec>::contains(const vec<dim, prec>& p) const
{
    //http://math.stackexchange.com/questions/4322/check-whether-a-point-is-within-a-3d-triangle
    //todo: if point is not even on triangle plane
    double area2 = length(b - a) * length(c - a);
    double a = length(b - p) * length(c - p) / area2;
    double b = length(c - p) * length(a - p) / area2;
    double c = 1 - a - b;

    if(a < 0 || a > 1) return 0;
    if(b < 0 || b > 1) return 0;
    if(c < 0 || c > 1) return 0;

    return 1;
}

//typedefs
template<typename prec> using triangle2 = triangle<2, prec>;
template<typename prec> using triangle3 = triangle<3, prec>;
template<typename prec> using triangle4 = triangle<4, prec>;

using triangle2f = triangle<2, float>;
using triangle3f = triangle<3, float>;
using triangle4f = triangle<4, float>;

using triangle2i = triangle<2, int>;
using triangle3i = triangle<3, int>;
using triangle4i = triangle<4, int>;

using triangle2ui = triangle<2, unsigned int>;
using triangle3ui = triangle<3, unsigned int>;
using triangle4ui = triangle<4, unsigned int>;

//operator
template<size_t dim, typename prec> std::ostream& operator<<(std::ostream& os, const triangle<dim, prec>& obj)
{
    os << obj.a << " " << obj.b << " " << obj.c;
    return os;
}

}
