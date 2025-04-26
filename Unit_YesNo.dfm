object Form_YesNo: TForm_YesNo
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'MQMinsweeper_Start_New_Game'
  ClientHeight = 172
  ClientWidth = 435
  Color = 1052688
  Font.Charset = DEFAULT_CHARSET
  Font.Color = 15790320
  Font.Height = -21
  Font.Name = 'Courier New'
  Font.Style = [fsBold]
  Position = poScreenCenter
  TextHeight = 23
  object Label_Text: TLabel
    Left = 30
    Top = 30
    Width = 325
    Height = 46
    Caption = 'Are you sure you want to start a new game?'
    WordWrap = True
  end
  object Panel_Yes: TPanel
    Left = 20
    Top = 110
    Width = 185
    Height = 41
    BevelOuter = bvNone
    Caption = 'Yes'
    Color = 1913483
    ParentBackground = False
    TabOrder = 0
    OnMouseDown = Panel_YesMouseDown
  end
  object Panel_No: TPanel
    Left = 226
    Top = 110
    Width = 185
    Height = 41
    BevelOuter = bvNone
    Caption = 'No'
    Color = 4210720
    ParentBackground = False
    TabOrder = 1
    OnMouseDown = Panel_NoMouseDown
  end
end
