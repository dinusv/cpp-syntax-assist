![c++ Snippet Assist](/doc/csa.png)

A configurable C and C++ code generator that can be used as an external plugin with most IDEs. The tool uses javascript code as a means of manipulating source files.

 * **Version**: 0.2.0
 * **License**: LPGL
 * **Website**: [csa.dinusv.com](http://csa.dinusv.com)
 * **Documentation**: [csa.dinusv.com/documentation](http://csa.dinusv.com/documentation)
 * **Build Requirements**
   * Qt 5.3 or higher
   * Libclang (LLVM 3.6 package or higher)


## How it works

The tool parses a selected source file, and, using clang library, creates an Abstract Syntax Tree (AST) Model which is then exposed to javascript functions. Think similarly to how the Document Object Model (DOM) is used in interacting with HTML and XML files, the AST Model can provide the same convention when dealing with C or C++ files. This can become very useful when one wishes to insert predefined chunks of code within a project.  

Functionality is added through javascript files as a form of plugins. Each file or plugin can contain one or more sets of functions that manipulate the file. The functions can be called through command line arguments, or by using  C++ Snippet Assists GUI. 

To further clarify, let's look at an actual example. Consider the following C++ header file:

```C++

class SimpleExample{
public:
	SimpleExample(){}
};

```

Now, let's assume we would like to wrap this class within the 'example' namespace, and that there will be more classes that will require that, so we need to provide a means to do it automatically. By using C++ Snippet Assist we can parse the file, and create an AST Model that can be accessed by javascript plugins. To create such a plugin, we simply create a javascript file in C++ Snippet Assist's plugin directory, and create a simple function we can call later. So we need to setup the file in *plugins/namespace.js*, and then we can add the function *wrapNamespace()*:

```js

function wrapNamespace(){

	// from the codebase, we select the main file received as a command line argument
	var fileNode = codeBase.files()[0]; 

	// we find the first class within the file
	var classNode = fileNode.find('class');
	
	// we insert the namespace before and after the node
	classNode.before('namespace example{\n');
	classNode.afterln('\n}');

	// we save the work and write the modified contents
	codeBase.save();
}


```

I will describe the above function in detail later on, but for now I'd like to show how we can use it. We can open a file from the command line with C++ Snippet Assist: 

```

./cppsnippetassist "simpleexample.h"

```

Then we can call our function by typing it in the command section:

 ![Command section screen shot](/doc/screen-simpleexample.jpg)


It's also possible to call this function directly from the command line, we just need to set the execute (-e) parameter together with the force (-f) flag: 

```

./cppsnippetassist "simpleexampe.h" -e "wrapNamespace()" -f

```

Both of these methods will wrap our class with the example namespace.

## Installation and usage

To start using C++ Snippet assist, simply download the latest version from the release section, and extract the files using your favorite decompression utility.

### Standalone Usage

The main executable is called **cppsnippetassist**. To launch it, you need to call it from the command line with a *c* or *c++* file as an argument. A set of optional arguments is available:

```

cppsnippetassist <file> [-c <offset> -lc <line> <column> -e <command> -f]

``` 

 * The '-c' option sets the *user cursor* at the specified <offset> position within the file. The user cursor is basically where you want *CSA* to focus a node. When using CSA as a plugin, most IDE's allow you to send the position where the user is editing text as a command line argument to an external plugin. This is ideal if you want to generate code at that specified location. You can obtain the focus node within the javascript plugin by querying the *codeBase*. 
 * The '-lc' option sets the *user cursor* at the specified <line> number and <column> position within the file.
 * The '-e' option loads the <command> text within the command section of the user interface.
 * The '-f' flag executes the loaded command without loading the user interface. 

### Installation with QTCreator



### Installation with Visual Studio


## Plugins

