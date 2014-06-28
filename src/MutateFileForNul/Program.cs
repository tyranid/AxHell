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
using System.Security.Cryptography;
using System.IO;

namespace MutateFileForNul
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 5)
            {
                Console.WriteLine("Usage MutateFileForNul input.exe offset length nulpos output.exe");
            }
            else
            {
                byte[] input = File.ReadAllBytes(args[0]);
                byte[] mutate = new byte[int.Parse(args[2])];
                int ofs = int.Parse(args[1]);
                int pos = int.Parse(args[3]);
                Random r = new Random(12345678);

                while(true)
                {
                    r.NextBytes(mutate);
                    Array.Copy(mutate, 0, input, ofs, mutate.Length);

                    SHA1 s = SHA1.Create();

                    byte[] hash = s.ComputeHash(input);
                    if (hash[pos] == 0)
                    {
                        File.WriteAllBytes(args[4], input);
                        Console.WriteLine("Found hash");
                        break;
                    }
                }
            }
        }
    }
}
