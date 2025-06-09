using System.Data.Entity;

namespace MasterPaul
{
    internal class AppDBContext : DbContext
    {
        public AppDBContext() : base ("name=Master_PaulEntities") { }
        public DbSet<Material_type_import> material_Type_Imports { get; set; }
        ...
    }
}

using System.Windows.Forms;
namespace MasterPaul
{
    public partial class PartnerControl : UserControl
    {
        public int PartnerID { get; set; }
        public PartnerControl()
        {
            InitializeComponent();
        }
        public void DataSet(string partner_type, string partner_name, string director, string phone, string rait, double percent)
        {
            try
            {
                label1.Text = partner_type;
                label2.Text = partner_name;
                label3.Text = director;
                label4.Text = "+7 " + phone;
                label5.Text = "Рейтинг: " + rait;
                label6.Text = percent + "%";
            }
            catch (Exception ex)
            {
                Console.WriteLine("Ошибка инициализации данных: " + ex.Message);
            }
        }
        private void PartnerControl_Click(object sender, EventArgs e)
        {
                var updateForm = new update(PartnerID);
                updateForm.FormClosed += (s, args) => Application.Exit();
                this.FindForm().Hide();
                updateForm.Show();
        }

    }
}
namespace MasterPaul
{
    public partial class Form1 : Form
    {
        private AppDBContext dbContext = new AppDBContext();
        public Form1()
        {
            InitializeComponent();
            LoadPartners();
        }

        public void LoadPartners()
        {
                flowLayoutPanel1.Controls.Clear();
                var partners = dbContext.partners_Imports.ToList();

                foreach (var partner in partners)
                {
                        var obshiyObem = dbContext.partner_Products_Imports
                            .Where(r => r.ID_партнера == partner.ID_партнера)
                            .Sum(r => r.Количество_продукции);

                        double precent = obshiyObem >= 300000 ? 15 :
                                        obshiyObem >= 50000 ? 10 :
                                        obshiyObem >= 10000 ? 5 : 0;

                        var tipPartnera = dbContext.partners_Types
                            .FirstOrDefault(t => t.ID_типа == partner.Тип_партнера)?.Тип_партнера
                            ?? "Неизвестный тип";

                        PartnerControl partnerControl = new PartnerControl();
                        partnerControl.PartnerID = partner.ID_партнера;
                        partnerControl.DataSet(
                            tipPartnera,
                            partner.Наименование_партнера,
                            partner.Директор,
                            partner.Телефон_партнера,
                            partner.Рейтинг,
                            precent
                        );

                        flowLayoutPanel1.Controls.Add(partnerControl);
                }
        }
    }
}
namespace MasterPaul
{
    public partial class insert : Form
    {
        private AppDBContext dbContext = new AppDBContext();

        public insert()
        {
            InitializeComponent();
            LoadPartnerTypes();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            var main = new Form1();
            main.FormClosed += (s, args) => Application.Exit();
            this.FindForm().Hide();
            main.Show();
        }

        private void LoadPartnerTypes()
        {
            using (var db = new AppDBContext())
            {
                var partnerTypes = db.partners_Types
                                    .OrderBy(t => t.Тип_партнера)
                                    .ToList();

                comboBox1.DataSource = partnerTypes;
                comboBox1.DisplayMember = "Тип_партнера"; 
                comboBox1.ValueMember = "ID_типа";     
                comboBox1.SelectedIndex = -1;     
            }
        }

        private void insert_Click(object sender, EventArgs e)
        {
                
            if (!int.TryParse(textBox2.Text, out int рейтинг)  рейтинг < 0  рейтинг == -0)
            {
                MessageBox.Show("Рейтинг должен быть целым неотрицательным числом!", "Ошибка",
                            MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            var newPartner = new Partners_import
            {
                Наименование_партнера = textBox1.Text,
                Тип_партнера = (int)comboBox1.SelectedValue,
                Рейтинг = textBox2.Text,
                Юридический_адрес_партнера = textBox3.Text,
                Директор = textBox4.Text,
                Телефон_партнера = textBox5.Text,
                Электронная_почта_партнера = textBox6.Text
            };

            using (var db = new AppDBContext())
            {
                db.partners_Imports.Add(newPartner);
                db.SaveChanges();
            }

            ClearFormFields();
        }

        private void ClearFormFields()
        {
            textBox1.Clear();
            comboBox1.SelectedIndex = -1;
            textBox2.Clear();
            textBox3.Clear();
            textBox4.Clear();
            textBox5.Clear();
            textBox6.Clear();
        }
    }
}

namespace MasterPaul
{
    public partial class update : Form
    {
        private int partnerId;

        public update(int partnerId)
        {
            InitializeComponent();
            this.partnerId = partnerId;

            LoadPartnerData();
            LoadPartnerTypes();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            var main = new Form1();
            main.FormClosed += (s, args) => Application.Exit();
            this.FindForm().Hide();
            main.Show();
        }

        private void LoadPartnerTypes()
        {
            using (var db = new AppDBContext())
            {
                var partnerTypes = db.partners_Types
                                    .OrderBy(t => t.Тип_партнера)
                                    .ToList();

                comboBox1.DataSource = partnerTypes;
                comboBox1.DisplayMember = "Тип_партнера";
                comboBox1.ValueMember = "ID_типа";
            }
            
        }

        private void LoadPartnerData()
        {
            var dbContext = new AppDBContext();
            var partner = dbContext.partners_Imports.FirstOrDefault(p => p.ID_партнера == partnerId);

            if (partner != null)
            {
                textBox1.Text = partner.Наименование_партнера;
                comboBox1.SelectedValue = partner.Тип_партнера;
                textBox2.Text = partner.Рейтинг;
                textBox3.Text = partner.Юридический_адрес_партнера;
                textBox4.Text = partner.Директор;
                textBox5.Text = partner.Телефон_партнера;
                textBox6.Text = partner.Электронная_почта_партнера;

            }
        }

        private void button1_Click(object sender, EventArgs e)
        {

            if (!int.TryParse(textBox2.Text, out int рейтинг)  рейтинг < 0 || рейтинг == -0)
            {
                MessageBox.Show("Рейтинг должен быть целым неотрицательным числом!", "Ошибка",
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            using (var dbContext = new AppDBContext())
            {
                var partner = dbContext.partners_Imports.FirstOrDefault(p => p.ID_партнера == partnerId);

                if (partner != null)
                {
                    partner.Наименование_партнера = textBox1.Text;
                    partner.Тип_партнера = Convert.ToInt32(comboBox1.SelectedValue);
                    partner.Рейтинг = textBox2.Text;
                    partner.Юридический_адрес_партнера = textBox3.Text;
                    partner.Директор = textBox4.Text;
                    partner.Телефон_партнера = textBox5.Text;
                    partner.Электронная_почта_партнера = textBox6.Text;

                    dbContext.SaveChanges();
                    MessageBox.Show("Данные успешно сохранены.", "Успех",
                        MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    MessageBox.Show("Партнёр не найден.", "Ошибка",
                        MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }
    }
}
using System;
using System.Data;
using System.Linq;
using System.Windows.Forms;
using System.Data.Entity;

namespace MasterPaul
{
    public partial class HistoryForm : Form
    {
        private readonly AppDBContext db = new AppDBContext();

        public HistoryForm()
        {
            InitializeComponent();
            InitializeControls();
            LoadPartners();
        }

        private void LoadPartners()
        {
            var partners = db.partners_imports
                .OrderBy(t => t.partner_name)
                .ToList();

            cmbPartners.DisplayMember = "partner_name";
            cmbPartners.ValueMember = "id";
            cmbPartners.DataSource = partners;
        }

        private void LoadHistory(int partnerId)
        {
            
            var history = db.product_partners_imports
                .Where(ppi => ppi.PartnerId == partnerId)
                .Select(ppi => new
                {
                    ppi.Id,
                    ppi.ProductName,
                    ppi.ImplementationDate,
                    ppi.Quantity,
                    ppi.Status
                })
                .ToList();

            dataGridView1.DataSource = history;
        }

        private void CmbPartners_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cmbPartners.SelectedValue != null)
            {
                int partnerId = (int)cmbPartners.SelectedValue;
                LoadHistory(partnerId);
            }
        }

        private void BtnRefresh_Click(object sender, EventArgs e)
        {
            if (cmbPartners.SelectedValue != null)
            {
                int partnerId = (int)cmbPartners.SelectedValue;
                LoadHistory(partnerId);
            }
        }

         private void button2_Click(object sender, EventArgs e)
        {
            var main = new Form1();
            main.FormClosed += (s, args) => Application.Exit();
            this.FindForm().Hide();
            main.Show();
        }
    }
} 
