/*
 * util.h
 *
 *  Created on: 29 θών. 2018 γ.
 *      Author: feelt
 */

#ifndef UTIL_H_
#define UTIL_H_

#define max(a, b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a, b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#define constrain(x, a, b) \
	({ __typeof__ (x) _x = (x); \
       __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _x ? _a : (_b < _x ? _b : x); })


#define map(x, in_min, in_max, out_min, out_max) \
	({ __typeof__ (x) _x = (x); \
       __typeof__ (in_min) _in_min = (in_min); \
       __typeof__ (in_max) _in_max = (in_max); \
       __typeof__ (out_min) _out_min = (out_min); \
       __typeof__ (out_max) _out_max = (out_max); \
     (_x - _in_min) * (_out_max - _out_min) / (_in_max - _in_min) + _out_min; })

#define middle_of_3(a, b, c) (a <= b && a <= c) ? ((b <= c) ? b : c) : ((b <= a && b <= c) ? ((a <= c) ? a : c) : ((a <= b) ? a : b))

#define PI 3.14159265
#define DEG_TO_RAD 0.01745329
#define RAD_TO_DEG 57.2957786

#endif /* UTIL_H_ */
