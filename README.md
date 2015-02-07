# C++ Snippet Assist

A configurable C and C++ code generator that can be used as an external plugin with most IDEs. The tool uses javascript code as a means of manipulating source files.

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

Now, let's assume we would like to wrap this class within the 'example' namespace, and that there will be more classes that will require that, so we need to provide a means to do it automatically. By using C++ Snippet Assist we can parse the file, and create an AST Model that can be accessed by javascript plugins. To create such a plugin, we simply create a javascript file in C++ Snippet Assist's plugin directory, and create a simple function we can call later. So we can setup the file in *plugins/namespace.js*, we can add the function *wrapNamespace()*:

```js

function wrapNamespace(){

	// from the codebase, we select the main file received as a command line argument
	var fileNode = codeBase.files()[0]; 

	// we find the first class within the file
	var classNode = fileNode.find('class');
	
	// we insert the namespace before and after the node
	first.before('namespace example{\n');
	last.afterln('\n}');

	// we save the work
	codeBase.save();
}


```



