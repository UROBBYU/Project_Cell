using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace ProjectCell
{
    public partial class Form1 : Form
    {
        private string selectedPort = "COM";
        private string[] buttonTexts = {
            "First",
            "Second",
            "Third",
            "Fourth",
            "Fifth",
            "Sixth",
            "Seventh",
            "Eighth",
            "Nineth",
            "Tenth",
            "Eleventh",
            "Twelveth",
            "Thirteenth"
        };

        public string[] updatePorts()
        {
            string[] portnames = SerialPort.GetPortNames();
            string[] comnames = new string[COMMenu.DropDownItems.Count];
            for (int i = 0; i < COMMenu.DropDownItems.Count; i++)
            {
                comnames[i] = COMMenu.DropDownItems[i].Name;
            }
            if (COMMenu.DropDownItems.Count == 0 || !comnames.SequenceEqual(portnames))
            {
                COMMenu.DropDownItems.Clear();
                for (int i = 0; i < portnames.Length; i++)
                {
                    ToolStripMenuItem com = new ToolStripMenuItem();
                    com.Name = portnames[i];
                    com.Text = portnames[i];
                    com.Click += new System.EventHandler(com_Click);
                    COMMenu.DropDownItems.Add(com);
                }
                if (!portnames.Contains(selectedPort))
                {
                    selectedPort = "COM";
                    serialPort1.Close();
                }
            }
            COMMenu.Text = selectedPort;
            return portnames;
        }

        public Form1()
        {
            InitializeComponent();
            Button[] button = new Button[13];
            for (int i = 0; i < 13; i++)
            {
                button[i] = new Button();
                button[i].Name = "Button" + (i + 1);
                button[i].BackColor = SystemColors.ControlDarkDark;
                button[i].ForeColor = Color.White;
                button[i].FlatStyle = FlatStyle.Flat;
                button[i].FlatAppearance.BorderSize = 1;
                button[i].FlatAppearance.BorderColor = SystemColors.ButtonShadow;
                button[i].FlatAppearance.MouseOverBackColor = SystemColors.ActiveBorder;
                button[i].FlatAppearance.MouseDownBackColor = SystemColors.ActiveCaption;
                button[i].Size = new Size(789,50);
                button[i].Text = buttonTexts[i];
                button[i].Font = new Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold);
                button[i].Cursor = Cursors.Hand;
                button[i].Click += new EventHandler(button_Click);
            }
            flowLayoutPanel1.Controls.AddRange(button);
            back1.RunWorkerAsync();
        }
        
        private void button_Click(object sender, EventArgs e)
        {
            if (!serialPort1.IsOpen || selectedPort == "COM")
            {
                MessageBox.Show("Invalid serial port!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                Button button = sender as Button;
                int num = int.Parse(button.Name.Substring(6));
                serialPort1.Write(num.ToString());
            }
        }

        private void com_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen) serialPort1.Close();
            ToolStripMenuItem com = sender as ToolStripMenuItem;
            selectedPort = com.Name;
            serialPort1.PortName = selectedPort;
            serialPort1.Open();
        }

        private void incomeData(object sender, EventArgs e)
        {
            string msg = serialPort1.ReadExisting();
            switch (msg)
            {
                case "connect":
                    MessageBox.Show("Successfully connected", "Message", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    break;

                case "correct":
                    MessageBox.Show("Correct answer", "Message", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    break;

                case "wrong":
                    MessageBox.Show("Wrong answer", "Message", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    break;

                default:
                    MessageBox.Show("Invalid data!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    break;
            }
        }

        private void serialErr(object sender, EventArgs e)
        {
            serialPort1.Close();
            MessageBox.Show("Serial port error!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void formClose(object sender, EventArgs e)
        {
            serialPort1.Close();
            back1.CancelAsync();
        }

        private void closeButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void drag(object sender, EventArgs e)
        {
            base.Capture = false;
            Message m = Message.Create(base.Handle, 0xa1, new IntPtr(2), IntPtr.Zero);
            this.WndProc(ref m);
        }
        
        private void backwork(object sender, EventArgs e)
        {
            while (true)
            {
                System.Threading.Thread.Sleep(500);
                updatePorts();
            }
        }
    }
}
