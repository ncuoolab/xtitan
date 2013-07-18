/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#include "CheckPoint.hpp"

using xtitan::AsyncCheckPoint;
using xtitan::CheckPoint;


bool CheckPoint::operator ==( const CheckPoint & that ) const {
	return this->id == that.id && this->args == that.args;
}

bool AsyncCheckPoint::operator ==( const AsyncCheckPoint & that ) const {
	return this->id == that.id && this->pre == that.pre && this->args == that.args;
}
