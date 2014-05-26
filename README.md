# C++ Snippet Assist

A configurable c/c++ code generator that can be used as an external plugin with all IDE's that support this feature: QT Creator, Visual Studio, etc.

The generator parses a selected source file, creating *hooks* within the file according to its contents, then by a set of configured instructions, code can be automatically generated at the location specified by the *hooks*.

Libclang is used to parse the c++ code and create the hooks, while a javascript file is used to configure the generator statements, which can then be accessed by a simple user interface written in qml.

## How it works

Given a file *csaexception.h* :

```
#include <exception>
#include <string>

namespace csa{

class CSAException : public std::exception{

public:
    CSAException( const std::string& message );
    virtual ~CSAException() throw() {}

    virtual const std::string& message() const throw();

private:
    std::string m_message;
};

inline const std::string& CSAException::message() const throw(){
    return m_message;
}

}// namespace
```

Within c++ snippet assist, the following hooks will be created : 

```
// HOOK_FILE_BEGIN
#include <exception>
#include <string>

namespace csa{
// HOOK_NAMESPACE_BEGIN

class CSAException : public std::exception{
// HOOK_CLASS_BEGIN

public:
    CSAException( const std::string& message );
    virtual ~CSAException() throw() {}

    virtual const std::string& message() const throw();

	// HOOK_CLASS_PUBLIC_SECTION
	// HOOK_CLASS_PROTECTED_SECTION
private:
    std::string m_message;

	// HOOK_CLASS_PRIVATE_SECTION
// HOOK_CLASS_END
};
// HOOK_CLASS_IMPLEMENTATION

inline const std::string& CSAException::message() const throw(){
    return m_message;
}

// HOOK_NAMESPACE_END
}// namespace
// HOOK_FILE_END
```

The javascript file can be configured to generate code for files, namespaces and classes. We can configure a function that will generate a getter method for a sent property to its argument list :

```JavaScript
var csaclass = {
    
	'add' : {
        'getter' : function(options){
            // options is an array of parameters
            var varName = options[0];
            var varType = options[1];
            codeBase.insert({
				// Generate code at the public section of the class
                'PUBLIC' :  varType + ' ' + varName + '() const{ return m_' + varName + ';}\n'
            });
        }
	}
}

```

