using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NLog;
using YamlDotNet.Serialization;

namespace YamlGenerator
{
    public static class Generator
    {
        private static Logger g = LogManager.GetCurrentClassLogger();

        public static void Generate(ModisSetting setting)
        {
            foreach (string product in setting.SelectedProducts)
            {
                //我知道是对亮温进行处理
                if (product.Contains("02"))
                {
                    //获取数据类别
                    string type = product.Substring(0, 3);
                    //获取是否需要归档
                    bool needArchive = setting.ArchiveSetting != null;
                    bool needPreprocess = setting.PreprocessSetting.PreprocessBt != null;
                    if (needArchive)
                    {
                        OptionsPreprocessBt options = new OptionsPreprocessBt();
                        options.NeedArchive = true;
                        options.NeedPreprocess = false;
                        options.MODISDataPath = setting.ArchiveSetting.ModisDataFolder;
                        options.WorkspacePath = setting.WorkSpaceFolder;
                        options.NeedPreprocess = false;
                        options.Type = type;
                        options.Date = "2001001";
                        options.MinLon = 0;
                        options.MaxLon = 0;
                        options.MinLat = 0;
                        options.MaxLat = 0;
                        options.Band = "";
                        options.MRTKernelType = "";
                        options.MRTProjectionType = "";
                        options.MRTProjectionArgs = "";
                        options.MRTPixelSize = 0;
                        options.ResultDataSetPath = "";
                        options.SaveTo($"{options.WorkspacePath}Tmp\\Archive_bt.yml");
                        g.Info($"Generate {options.WorkspacePath}Tmp\\Archive_bt.yml");
                    }

                    //获取是否需要对亮温进行预处理
                    if (needPreprocess)
                    {
                        var dateStart = GetDate(setting.StartDate);
                        var dateEnd = GetDate(setting.EndDate);
                        while (dateStart <= dateEnd)
                        {
                            OptionsPreprocessBt options = new OptionsPreprocessBt();
                            options.NeedArchive = false; //预处理时不再归档
                            options.MODISDataPath = needArchive ? setting.ArchiveSetting.ModisDataFolder : "";
                            options.WorkspacePath = setting.WorkSpaceFolder;
                            options.NeedPreprocess = true;
                            options.Type = type;
                            options.Date = GetDOYDateStr(dateStart);

                            options.MinLon = setting.PreprocessSetting.PreprocessBt.MinLon;
                            options.MaxLon = setting.PreprocessSetting.PreprocessBt.MaxLon;
                            options.MinLat = setting.PreprocessSetting.PreprocessBt.MinLat;
                            options.MaxLat = setting.PreprocessSetting.PreprocessBt.MaxLat;
                            options.Band = setting.PreprocessSetting.PreprocessBt.Band;
                            options.MRTKernelType = setting.PreprocessSetting.PreprocessBt.MRTKernelType;
                            options.MRTProjectionType = setting.PreprocessSetting.PreprocessBt.MRTProjectionType;
                            options.MRTProjectionArgs = setting.PreprocessSetting.PreprocessBt.MRTProjectionArgs;
                            options.MRTPixelSize = setting.PreprocessSetting.PreprocessBt.MRTPixelSize;
                            options.ResultDataSetPath =
                                setting.WorkSpaceFolder +
                                $"Standard\\{type}_BT\\{dateStart.Year}\\bt_{options.Date}.tif";
                            options.SaveTo($"{options.WorkspacePath}Tmp\\Preprocess_bt_{options.Date}.yml");
                            g.Info($"Generate {options.WorkspacePath}Tmp\\Preprocess_bt_{options.Date}.yml");
                            dateStart = dateStart.AddDays(1);
                        }
                    }
                }


            }
        }

        public static string GetDOYDateStr(DateTime date)
        {
            var firstDay = new DateTime(date.Year, 1, 1);
            var daySpan = (date - firstDay).Days;
            var doy = string.Empty;
            if (daySpan < 10)
                doy = "00" + daySpan;
            else if (daySpan >= 10 && daySpan <= 99)
                doy = "0" + daySpan;
            else
                doy = daySpan.ToString();
            return date.Year.ToString() + doy;
        }

        public static DateTime GetDate(string dateStr)
        {
            g.Debug($"Convert DateStr {dateStr} to DateTime..");

            var year = Convert.ToInt32(dateStr.Substring(0, 4));
            g.Debug($"year: {year}");

            if (dateStr.Length == 7) //DOY模式
            {
                int doy = Convert.ToInt32(dateStr.Substring(4, 3).TrimStart('0'));
                g.Debug("doy: " + doy);

                var ans = new DateTime(year, 1, 1).AddDays(doy);
                g.Info($"DateStr:{dateStr} to DateTime:{ans.Date.ToShortDateString()}");
                return ans;
            }

            if (dateStr.Length == 8) //YYYYMMDD模式
            {
                var month = Convert.ToInt32(dateStr.Substring(4, 2).TrimStart('0'));
                var day = Convert.ToInt32(dateStr.Substring(6, 2).TrimStart('0'));
                var ans = new DateTime(year, month, day);
                g.Info($"DateStr:{dateStr} to DateTime:{ans.Date.ToShortDateString()}");
                return ans;
            }

            throw new ArgumentException("wrong datestr format!");
        }
    }


}
