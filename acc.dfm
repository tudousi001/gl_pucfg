object AccForm: TAccForm
  Left = 524
  Top = 192
  BorderStyle = bsDialog
  Caption = #36134#21495#35774#32622
  ClientHeight = 217
  ClientWidth = 371
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label213: TLabel
    Left = 56
    Top = 128
    Width = 41
    Height = 13
    AutoSize = False
    Caption = #26435#38480
  end
  object Label206: TLabel
    Left = 56
    Top = 96
    Width = 41
    Height = 13
    AutoSize = False
    Caption = #35828#26126
  end
  object Label179: TLabel
    Left = 56
    Top = 64
    Width = 41
    Height = 13
    AutoSize = False
    Caption = #23494#30721
  end
  object Label177: TLabel
    Left = 56
    Top = 32
    Width = 41
    Height = 13
    AutoSize = False
    Caption = #36134#21495
  end
  object Level: TComboBox
    Left = 111
    Top = 128
    Width = 145
    Height = 21
    ItemHeight = 13
    ItemIndex = 1
    TabOrder = 0
    Text = #26222#36890#29992#25143
    Items.Strings = (
      #21463#38480#29992#25143
      #26222#36890#29992#25143
      #36229#32423#29992#25143)
  end
  object Acc: TEdit
    Left = 111
    Top = 30
    Width = 146
    Height = 21
    MaxLength = 15
    TabOrder = 1
  end
  object Pwd: TEdit
    Left = 111
    Top = 62
    Width = 146
    Height = 21
    MaxLength = 15
    PasswordChar = '*'
    TabOrder = 2
  end
  object Des: TEdit
    Left = 111
    Top = 94
    Width = 146
    Height = 21
    MaxLength = 15
    TabOrder = 3
  end
  object Button1: TButton
    Left = 176
    Top = 176
    Width = 75
    Height = 25
    Caption = #30830#23450
    ModalResult = 1
    TabOrder = 4
  end
  object Button2: TButton
    Left = 272
    Top = 176
    Width = 75
    Height = 25
    Caption = #21462#28040
    ModalResult = 2
    TabOrder = 5
  end
end
