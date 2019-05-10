object SwitchDevForm: TSwitchDevForm
  Left = 451
  Top = 116
  BorderStyle = bsDialog
  Caption = #25511#21046#22120#35774#32622
  ClientHeight = 193
  ClientWidth = 302
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label60: TLabel
    Left = 24
    Top = 36
    Width = 57
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = #31867#22411
  end
  object Label35: TLabel
    Left = 24
    Top = 101
    Width = 57
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = #36890#36947#25968
  end
  object Label1: TLabel
    Left = 24
    Top = 69
    Width = 57
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = #35774#22791'ID'
  end
  object ChnNum: TCSpinEdit
    Left = 88
    Top = 96
    Width = 88
    Height = 22
    TabOrder = 2
    Value = 8
  end
  object Button1: TButton
    Left = 80
    Top = 152
    Width = 75
    Height = 25
    Caption = #30830#23450
    ModalResult = 1
    TabOrder = 3
  end
  object Button2: TButton
    Left = 179
    Top = 152
    Width = 75
    Height = 25
    Caption = #21462#28040
    ModalResult = 2
    TabOrder = 4
  end
  object Type: TComboBox
    Left = 88
    Top = 34
    Width = 88
    Height = 21
    ItemHeight = 13
    TabOrder = 0
  end
  object Id: TCSpinEdit
    Left = 88
    Top = 64
    Width = 88
    Height = 22
    MaxValue = 254
    TabOrder = 1
    Value = 254
  end
end
