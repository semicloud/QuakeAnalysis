using System;
using System.Collections.Generic;

namespace ModisDownloader
{
   public class Query
   {
       private List<string> _productNames;
       private DateTime _dateStart;
       private DateTime _dateStop;
       private string _dnbType;
       private double _minLng;
       private double _maxLng;
       private double _minLat;
       private double _maxLat;

       public List<string> ProductNames
       {
           get => _productNames;
           set => _productNames = value;
       }

       public DateTime DateStart
       {
           get => _dateStart;
           set => _dateStart = value;
       }

       public DateTime DateStop
       {
           get => _dateStop;
           set => _dateStop = value;
       }

       public string DnbType
       {
           get => _dnbType;
           set => _dnbType = value;
       }

       public double MinLng
       {
           get => _minLng;
           set => _minLng = value;
       }

       public double MaxLng
       {
           get => _maxLng;
           set => _maxLng = value;
       }

       public double MinLat
       {
           get => _minLat;
           set => _minLat = value;
       }

       public double MaxLat
       {
           get => _maxLat;
           set => _maxLat = value;
       }
   }
}
