using Microsoft.CSharp;
using System.CodeDom.Compiler;
using System.IO;
using TritonEngine;

namespace TritonEditorExtension
{
    public class CodeExtension
    {
        /// <summary>
        /// Compiles the specified code files into the specified assembly
        /// </summary>
        /// <param name="PathsToFiles">Array of file paths</param>
        /// <param name="PathToBuild">Path to build to</param>
        /// <param name="AssemblyName">The name of the assembly</param>
        /// <returns>Status of compilation</returns>
        public bool CompileCode(string[] PathsToFiles, string PathToBuild, string AssemblyName)
        {
            CSharpCodeProvider provider = new CSharpCodeProvider();

            CompilerParameters cp = new CompilerParameters();

            // Generate an executable instead of 
            // a class library.
            cp.GenerateExecutable = false;

            // Set the assembly file name to generate.
            cp.OutputAssembly = Path.Combine(PathToBuild, AssemblyName);

            // Create directory if it doesn't exist
            Directory.CreateDirectory(new FileInfo(cp.OutputAssembly).Directory.FullName);

            // Generate debug information.
            cp.IncludeDebugInformation = true;

            // Add an assembly reference.
            cp.ReferencedAssemblies.Add(System.Reflection.Assembly.GetAssembly(typeof(TritonEngine.TritonEngine)).Location);

            // Save the assembly as a physical file.
            cp.GenerateInMemory = false;

            // Set the level at which the compiler 
            // should start displaying warnings.
            cp.WarningLevel = 3;

            // Set whether to treat all warnings as errors.
            cp.TreatWarningsAsErrors = true;

            // Set compiler argument to optimize output.
            cp.CompilerOptions = "/optimize";

            // Set a temporary files collection.
            // The TempFileCollection stores the temporary files
            // generated during a build in the current directory,
            // and does not delete them after compilation.
            cp.TempFiles = new TempFileCollection(".", false);

            // Invoke compilation.
            CompilerResults cr = provider.CompileAssemblyFromFile(cp, PathsToFiles);

            bool noErrors = true;

            if (cr.Errors.Count > 0)
            {
                // Display compilation errors.
                foreach (CompilerError ce in cr.Errors)
                {
                    if (ce.IsWarning)
                    {
                        Debug.Log(ce.ToString(), LogSeverity.Warn);
                    }
                    else
                    {
                        noErrors = false;
                        Debug.Log(ce.ToString(), LogSeverity.Error);
                    }
                }
            }

            if (noErrors)
            {
                Debug.Log("Compiled successfully", LogSeverity.Info);
            }
            else
            {
                Debug.Log("Compilation failed", LogSeverity.Error);
            }

            return noErrors;
        }
    }
}
