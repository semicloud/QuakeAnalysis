using System.Diagnostics;
using System.Threading.Tasks;

namespace QuakeAnalysis
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
            return tcs.Task;
        }

        public static Task<int> RunProcessAsync(string fileName, string argument)
        {
            var tcs = new TaskCompletionSource<int>();

            var process = new Process
            {
                StartInfo =
                {
                    FileName = fileName,
                    Arguments = argument,
                    CreateNoWindow = false,
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
            return tcs.Task;
        }
    }
}
