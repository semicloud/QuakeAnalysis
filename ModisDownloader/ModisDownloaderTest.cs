using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Serialization;
using NUnit.Framework;
using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace ModisDownloader
{
    public static class ModisDownloaderTest
    {

        [Test]
        public async static Task TestInvokeService()
        {
            //var service =
            //@"https://modwebsrv.modaps.eosdis.nasa.gov/axis2/services/MODAPSservices/searchForFiles?product=MOD021KM&collection=61&start=2018-12-20&stop=2018-12-25&north=40&south=39&west=113&east=115&coordsOrTiles=coords&dayNightBoth=DNB";
            using (var client = new HttpClient())
            {
                var values = new List<KeyValuePair<string, string>>();
                values.Add(new KeyValuePair<string, string>("product", "MOD021KM"));
                values.Add(new KeyValuePair<string, string>("collection", "61"));
                values.Add(new KeyValuePair<string, string>("start", "2018-12-20"));
                values.Add(new KeyValuePair<string, string>("stop", "2018-12-25"));
                values.Add(new KeyValuePair<string, string>("north", "40"));
                values.Add(new KeyValuePair<string, string>("south", "39"));
                values.Add(new KeyValuePair<string, string>("west", "113"));
                values.Add(new KeyValuePair<string, string>("east", "115"));
                values.Add(new KeyValuePair<string, string>("coordsOrTiles", "coords"));
                values.Add(new KeyValuePair<string, string>("dayNightBoth", "DNB"));

                var content = new FormUrlEncodedContent(values);

                var response = await client.PostAsync(
                    "https://modwebsrv.modaps.eosdis.nasa.gov/axis2/services/MODAPSservices/searchForFiles", content);

                var responseString = await response.Content.ReadAsStringAsync();

                Console.WriteLine(responseString);
            }
        }

        //[Test]
        //public static async Task TestInvokeService2()
        //{
        //    //var service =
        //    //    @"https://modwebsrv.modaps.eosdis.nasa.gov/axis2/services/MODAPSservices/searchForFiles?product=MYD021KM&collection=61&start=2018-12-20&stop=2018-12-25&north=40&south=39&west=113&east=115&coordsOrTiles=coords&dayNightBoth=DNB";
        //    //using (var client = new HttpClient())
        //    //{
        //    //    string response = string.Empty;
        //    //    try
        //    //    {
        //    //        response = await client.GetStringAsync(service);
        //    //    }
        //    //    catch (HttpRequestException e)
        //    //    {
        //    //        Console.WriteLine(e.Message);
        //    //    }

        //    //    Console.WriteLine("Response: " + response);

        //    //    var xmlDocument = new XmlDocument();
        //    //    xmlDocument.LoadXml(response.Trim());
        //    //    var nodes = xmlDocument.SelectNodes(@"//return");

        //    //    var fileIdList = new List<String>();
        //    //    if (nodes != null && nodes.Count > 0)
        //    //    {
        //    //        fileIdList = nodes.Cast<XmlNode>().ToList().Select(n => n.InnerText).ToList();
        //    //    }

        //    //    var filePropertiesServiceUrl = ServiceUtil.GetFilePropertiesServiceUrl(fileIdList);
        //    //    var fileUrlsServiceUrl = ServiceUtil.GetFileUrlsServiceUrl(fileIdList);

        //    //    var fileProperties = await client.GetStringAsync(filePropertiesServiceUrl);
        //    //    Console.WriteLine("File Properties:");
        //    //    Console.WriteLine(fileProperties);

        //    //    Console.WriteLine();

        //    //    var fileUrls = await client.GetStringAsync(fileUrlsServiceUrl);
        //    //    Console.WriteLine("File Urls:");
        //    //    Console.WriteLine(fileUrls);

        //    //    using (var reader = new StringReader(fileProperties))
        //    //    {
        //    //        var xmlSerializer = new XmlSerializer(typeof(getFilePropertiesResponse));
        //    //        var deserialize = (getFilePropertiesResponse)xmlSerializer.Deserialize(reader);
        //    //        Console.WriteLine(deserialize.Items.Length);
        //    //        foreach (var item in deserialize.Items)
        //    //        {
        //    //            Console.WriteLine(item.fileName);
        //    //        }
        //    //    }
        //    //}
        //}

        public static Stream GenerateStreamFromString(string s)
        {
            var stream = new MemoryStream();
            var writer = new StreamWriter(stream);
            writer.Write(s);
            writer.Flush();
            stream.Position = 0;
            return stream;
        }

        [Test]
        public static void TestLoadConfig()
        {
            var deserializer = new DeserializerBuilder().WithNamingConvention(new CamelCaseNamingConvention()).Build();
            var cfg =
                deserializer.Deserialize<ModisDownloaderConfig>(File.ReadAllText(@"modis_downloader_config.yaml"));
            Console.WriteLine(cfg.AppKey);
            Console.WriteLine(cfg.FileIdService);
        }

        [OneTimeSetUp]
        public static void RunBeforeAnyTests()
        {
            Console.WriteLine("I am here...");

            var dir = Path.GetDirectoryName(typeof(ModisDownloaderTest).Assembly.Location);
            if (dir != null)
            {
                Environment.CurrentDirectory = dir;
                Directory.SetCurrentDirectory(dir);
            }
            else
                throw new Exception("Path.GetDirectoryName(typeof(TestingWithReferencedFiles).Assembly.Location) returned null");
        }
    }
}
