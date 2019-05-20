using System.Diagnostics;
using System.Threading.Tasks;

namespace ModisEntity
{
    public static class CmdRunner
    {
        public static Task<int> RunProcessAsync(string fileName)
        {
            var tcs = new TaskCompletionSource<int>();

            var process = new Process
            {
                StartInfo =
                {
                    FileName = fileName,
                    CreateNoWindow = true,
                    UseShellExecute = false
                },
                EnableRaisingEvents = true
            };

            process.Exited += (sender, args) =>
            {
                tcs.SetResult(process.ExitCode);
                process.Dispose();
            };

            process.Start();
            //process.WaitForExit();
            return tcs.Task;
        }

        public static Task<int> RunProcessAsync(string filename, string arguments)
        {
            var tcs = new TaskCompletionSource<int>();
            var process = new Process
            {
                StartInfo =
                {
                    FileName = filename,
                    Arguments = arguments,
                    CreateNoWindow = true,
                    UseShellExecute = false,
                },
                EnableRaisingEvents = true
            };

            process.Exited += (sender, args) =>
            {
                tcs.SetResult(process.ExitCode);
                process.Dispose();
            };

            process.Start();
            //process.WaitForExit();
            return tcs.Task;
        }
    }
}
