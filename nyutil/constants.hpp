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

namespace nyutil
{

constexpr const double cPi = 3.14159265359;
constexpr const double cDeg = cPi / 180.0;
constexpr const double cE = 2.71828182845;

template<typename prec>
prec degrees(const prec& rad)
{
    return rad / cDeg;
}

template<typename prec>
prec radians(const prec& deg)
{
    return deg * cDeg;
}

#ifndef NYUTIL_CPP14_CONSTEXPR
    #if __cplusplus >= 201402L
        #define NYUTIL_CPP14_CONSTEXPR constexpr
    #else
        #define NYUTIL_CPP14_CONSTEXPR
    #endif
#endif //nyutil_cpp14_constexpr

}
