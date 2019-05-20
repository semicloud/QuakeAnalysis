using System;
using System.Collections.Generic;
using System.Drawing;
using System.Net;
using System.Threading;
using System.Threading.Tasks;

namespace ModisDownloader
{
    public static class ModisDownloaderExtendMethod
    {
        public static List<List<T>> Split<T>(this List<T> items, int sliceSize = 100)
        {
            List<List<T>> list = new List<List<T>>();
            for (int i = 0; i < items.Count; i += sliceSize)
                list.Add(items.GetRange(i, Math.Min(sliceSize, items.Count - i)));
            return list;
        }

        public static Font ChangeFontSize(this Font font, float fontSize)
        {
            if (font != null)
            {
                float currentSize = font.Size;
                if (currentSize != fontSize)
                {
                    font = new Font(font.Name, fontSize,
                        font.Style, font.Unit,
                        font.GdiCharSet, font.GdiVerticalFont);
                }
            }
            return font;
        }

        //public static async Task<byte[]> DownloadDataTaskAsync(this WebClient webClient, string address, CancellationToken cancellationToken)
        //{
        //    cancellationToken.ThrowIfCancellationRequested();
        //    using (cancellationToken.Register(webClient.CancelAsync))
        //    {
        //        return await webClient.DownloadDataTaskAsync(new Uri(address));
        //    }
        //}
    }
}
