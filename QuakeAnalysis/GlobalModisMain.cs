using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace QuakeAnalysis
{
    static class GlobalModisMain
    {
        private static ModisMainConfig _config;

        public static ModisMainConfig Config
        {
            get
            {
                if (_config != null) return _config;
                _config = ModisMainConfig.CreateInstance();
                return _config;
            }

            set => _config = value;
        }
    }
}
