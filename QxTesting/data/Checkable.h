/************************************************************************/
/* For Testing , all data inherit from this class
/************************************************************************/
#ifndef _CHECHABLE_H_
#define _CHECHABLE_H_

#include <string>

class Checkable {
public:
	virtual ~Checkable(){}

	virtual std::string toCheckString() const = 0;
};
#endif
