using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using DevExpress.Utils.Extensions;
using DevExpress.XtraEditors;
using DevExpress.XtraEditors.Repository;
using DevExpress.XtraTreeList;

namespace QuakeAnalysis
{
    public partial class Form2 : XtraForm
    {
        /// <summary>
        /// 这个是设置参数吧
        /// </summary>
        public Form2()
        {
            InitializeComponent();
            treeList1.CustomNodeCellEdit += TreeList1OnCustomNodeCellEdit;
        }

        private void TreeList1OnCustomNodeCellEdit(object sender, GetCustomNodeCellEditEventArgs e)
        {
            //var repositoryItemHyperLinkEdit = new RepositoryItemHyperLinkEdit();
            //if (e.Node.GetValue("设置参数") != null)
            //{
            //    e.RepositoryItem = repositoryItemHyperLinkEdit;
            //}
        }

        private void Form2_Load(object sender, EventArgs e)
        {
            treeList1.OptionsView.ShowIndicator = false;
            treeList1.ExpandAll();
            treeList1.OptionsView.EnableAppearanceEvenRow = true;
            treeList1.OptionsView.ShowVertLines = true;

            var repositoryItemHyperLinkEdit = new RepositoryItemHyperLinkEdit { SingleClick = true };
            repositoryItemHyperLinkEdit.OpenLink += (o, args) =>
            {
                var focusedNode = treeList1.FocusedNode;
                // 点在父节点（即分类节点）上直接返回
                if (focusedNode == null) return;
                if (focusedNode.HasChildren) return;
                if (focusedNode.CheckState == CheckState.Checked)
                {
                    var displayText = focusedNode.GetDisplayText(0);
                    if (displayText.Equals("四分位法"))
                    {
                        var frmSetQuantile = new FrmSetQuantile();
                        frmSetQuantile.ShowDialog(this);
                    }
                    else if (displayText.Equals("滑动平均法"))
                    {
                        var frmSetSlideMean = new FrmSetSlideMean();
                        frmSetSlideMean.ShowDialog(this);
                    }
                    else if (displayText.Equals("矢量等值线生成"))
                    {
                        var frmSetContour = new FrmSetContour();
                        frmSetContour.ShowDialog(this);
                    }
                    else if (displayText.Equals("栅格专题图"))
                    {
                        var frmSetRasterMap = new FrmSetRasterMap();
                        frmSetRasterMap.ShowDialog(this);
                    }
                }
            };

            treeListColumn2.ColumnEdit = repositoryItemHyperLinkEdit;

            gridControl1.DataSource = LoadFileInfos();
            // gridcontrol无法通过设置百分比来设置列宽，只能使用BestFit方法自动优化
            // gridView1.OptionsView.ColumnAutoWidth = true;
            // gridView1.Columns.ForEach(c => c.OptionsColumn.FixedWidth = false);
            gridView1.BestFitColumns(true);

            progressBarControl1.EditValue = 60;
        }

        private List<FileInfo> LoadFileInfos()
        {
            var fileInfos = new List<FileInfo>
            {
                new FileInfo {Idx = 1, FileName = @"D:\Data\CODG1"},
                new FileInfo {Idx = 2, FileName = @"D:\Data\CODG2"}
            };
            return fileInfos;
        }

        /// <summary>
        /// 控制treelist节点checkbox的状态
        /// 父节点选中时，子节点全部选中
        /// 子节点全部选中时，父节点选中
        /// 子节点部分选中时，父节点部分选中
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void treeList1_AfterCheckNode(object sender, NodeEventArgs e)
        {
            if (e.Node.HasChildren)
            {
                if (e.Node.Checked)
                {
                    e.Node.CheckAll();
                }
                else if (!e.Node.Checked)
                {
                    e.Node.Nodes.ForEach(n => n.CheckState = CheckState.Unchecked);
                }
            }
            else
            {
                if (e.Node.ParentNode != null)
                {
                    if (e.Node.ParentNode.Nodes.All(n => n.Checked))
                        e.Node.ParentNode.CheckState = CheckState.Checked;
                    else if (e.Node.ParentNode.Nodes.Any(n => n.Checked))
                        e.Node.ParentNode.CheckState = CheckState.Indeterminate;
                    else e.Node.ParentNode.CheckState = CheckState.Unchecked;
                }
            }

        }


    }


}
