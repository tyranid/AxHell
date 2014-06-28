////////////////////////////////////////////////////////////////////////////////
// AxHell - A simple Exploitable ActiveX Control for RE/VR 
// Copyright (C) 2010 James Forshaw

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Security.Cryptography;

namespace FindSHA1Alias
{
    class Program
    {
        static bool IsMatch(byte[] alias, byte[] sign)
        {
            bool bRet = false;

            for (int i = 0; i < alias.Length; i++)
            {
                if (alias[i] != sign[i])
                {
                    break;
                }

                if (alias[i] == 0)
                {
                    bRet = true;
                    break;
                }
            }

            return bRet;
        }

        static void Main(string[] args)
        {
            if (args.Length < 3)
            {
                Console.WriteLine("Usage FindSHA1Alias filetohash filetofake output");
            }
            else
            {
                SHA1 s = SHA1.Create();
                byte[] hash = s.ComputeHash(File.ReadAllBytes(args[0]));

                byte[] file = File.ReadAllBytes(args[1]);
                byte[] data = new byte[file.Length+sizeof(ulong)];
                Array.Copy(file, data, file.Length);
                ulong v = 0;
                for (v = 0; v < ulong.MaxValue; v++)
                {
                    byte[] bytes = BitConverter.GetBytes(v);
                    Array.Copy(bytes, 0, data, file.Length, bytes.Length);

                    SHA1 sha1 = SHA1.Create();

                    byte[] comp = sha1.ComputeHash(data);
                    if (IsMatch(comp, hash))
                    {
                        StringBuilder builder = new StringBuilder();

                        foreach (byte b in comp)
                        {
                            builder.AppendFormat("{0:X02}", b);
                        }
                        Console.WriteLine("Match at {0} {1}", v, builder.ToString());
                        File.WriteAllBytes(args[2], data);
                        break;
                    }
                }
            }
        }
    }
}
