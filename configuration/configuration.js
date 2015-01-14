
var csanamespace = {
    
    'select' : {
        'namespace' : function(options){
            codeBase.select('namespace', options[0])
        },
        'class' : function(options){
            codeBase.select('class', options[0])
        },
        'root' : function(options){
            codeBase.select('root', '')
        }
    },
    'add' : {
        'namespace' : function(options){
            codeBase.insert({
                'BEGIN' : ' namespace ' + options[0] + '{ ',
                'END'   : ' }'
            });
        },
        'class' : function(options){
            codeBase.insert({
                'CURSOR' : 'class ' + options[0] + '{\n};'
            });
        }
    }

};

// helper function
function capitaliseFirstLetter(str){
    return str.charAt(0).toUpperCase() + str.slice(1);
}

var csaclass = {
    
    'select' : {
        'root' : function(options){
            codeBase.select('root', '')
        },
        'class' : function(options){
            codeBase.select('class', options[0])
        },
    },
    'add' : {
        'qproperty' : function(options){
            var className = codeBase.nodeIdentifier();
            var typeName  = options[0];
            var varName   = options[1];
            var varDeclaration = 'm_' + options[1];

            if ( options.length < 2 )
                throw 'Invalid options lenght. Usage : type.variable.ref.notifiable';

            var isReference  = false;
            var isNotifiable = false;
            for ( var i = 2; i < options.length; ++i ){
                if ( options[i] === "ref" )
                    isReference = true;
                else if  ( options[i] === "notifiable" )
                    isNotifiable = "true";
            }

            var qProperty  =
                    '\tQ_PROPERTY(' + typeName + ' ' + varName +
                    ' READ ' + varName + ' WRITE set' + capitaliseFirstLetter(varName) +
                    (isNotifiable ? (' NOTIFY ' + varName + 'Changed') : '') +  ')\n';

            var member     =
                    '\t' + typeName + ' m_' + varName + ';\n';

            var methodDecl =
                    '\t' + (isReference ? 'const ' : '') + typeName +
                    (isReference ? '& '   : ' ') + varName  +
                    '() const;\n' +

                    '\tvoid set' + capitaliseFirstLetter(varName) + '(' +
                    (isReference ? 'const ' : '') +
                    typeName +
                    (isReference ? '& ' : ' ') +
                    varName + ');\n';

            var methodImpl =
                    '\ninline ' + (isReference ? 'const ' : '') + typeName +
                    (isReference ? '& ' : ' ') + className + '::' + varName +
                    '() const{' + '\n\treturn m_' + varName + ';\n}\n\n' +

                    'inline void ' + className + '::set' + capitaliseFirstLetter(varName) +
                    '(' + (isReference ? 'const ' : '') + typeName + (isReference ? '& ' : '') +
                    varName + '){\n\t';
            if ( isNotifiable ){
                methodImpl +=
                        'if (m_' + varName  + ' != ' + varName + '){\n' +
                        '\t\tm_' + varName + ' = ' + varName + ';\n' +
                        '\t\temit ' + varName + 'Changed();\n' +
                        '\t}\n';
            } else {
                methodImpl +=
                        '\tm_' + varDeclaration + ' = ' + varDeclaration + ';\n';
            }
            methodImpl += '}\n';

            codeBase.insert({
                    'QPROPERTY' : qProperty,
                    'PRIVATE'   : member,
                    'PUBLIC'    : methodDecl,
                    'IMPLEMENTATION' : methodImpl
            });

        },
        'field' : function(options){
            
        }
    }
    
};

var csafile = {
    'select' : {
        'namespace' : function(options){
            if ( codeBase.select('namespace', options[0]) )
                csa = csaNamespace;
        },
        'class' : function(options){
            if ( codeBase.select('class', options[0]) )
                csa = csaClass;
        }
    },
    'add' : {
        'namespace' : function(options){
            
        },
        'class' : function(options){
            
        }
    }
};

//csa.add.namespace('ns');
//csa.select.namespace("demo");

//("evaluated");

// Instruction to array
/*

cas.add({
    "qmacro"  : "alksdmla"
    "public"  : "get/set"
    "private" : ""
});

Internal
Position pos = map[strIndex]
Reverse position to not influence


each object has position flags
class position flags { 
    "qmacro", "public", "protected", "private", "implementation", "cursor"
}
namespace position flags {
    "cursor", "end", "start"
}
file position flags{
    "start", "end"
}


*/
