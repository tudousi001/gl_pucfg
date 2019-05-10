object IPSetForm: TIPSetForm
  Left = 192
  Top = 130
  BorderStyle = bsDialog
  Caption = #19968#38190#20462#25913'IP'
  ClientHeight = 181
  ClientWidth = 362
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label3: TLabel
    Left = 20
    Top = 24
    Width = 49
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = '  IP '#22320#22336
  end
  object Label8: TLabel
    Left = 12
    Top = 56
    Width = 57
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = #23376#32593#25513#30721
  end
  object Label25: TLabel
    Left = 32
    Top = 88
    Width = 37
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = '  '#32593#20851
  end
  object IP: TEdit
    Left = 76
    Top = 20
    Width = 236
    Height = 19
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 0
    OnKeyPress = IPKeyPress
  end
  object IPMask: TEdit
    Left = 76
    Top = 52
    Width = 236
    Height = 19
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 1
    Text = '255.255.0.0'
  end
  object Gataway: TEdit
    Left = 76
    Top = 84
    Width = 236
    Height = 19
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 2
  end
  object Button1: TButton
    Left = 144
    Top = 128
    Width = 75
    Height = 25
    Caption = #30830#23450
    Default = True
    ModalResult = 1
    TabOrder = 3
  end
  object Button2: TButton
    Left = 240
    Top = 128
    Width = 75
    Height = 25
    Caption = #21462#28040
    ModalResult = 2
    TabOrder = 4
  end
end
