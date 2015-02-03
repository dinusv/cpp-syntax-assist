#ifndef TESTCLASS_HPP
#define TESTCLASS_HPP

#include <QString>

class TestClass{

    TestClass();
	~TestClass();

private:
	int m_name;

public:
	const int& name() const;
	void setName(const int& name);
};

TestClass::~TestClass(){
}

inline const int& TestClass::name() const{
	return m_name;
}

inline void TestClass::setName(const int& name){
	if (m_name != name){
		m_name = name;
		emit nameChanged();
	}
}

#endif // TESTCLASS_HPP
