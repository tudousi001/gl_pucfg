object SwitchForm: TSwitchForm
  Left = 566
  Top = 344
  BorderStyle = bsDialog
  Caption = #24320#20851#35774#32622
  ClientHeight = 237
  ClientWidth = 428
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
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
    Top = 69
    Width = 57
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = #36890#36947#21495
  end
  object Label73: TLabel
    Left = 200
    Top = 37
    Width = 57
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = #24320#20851#34892#31243
  end
  object Label2: TLabel
    Left = 216
    Top = 104
    Width = 49
    Height = 13
    AutoSize = False
    Caption = #25511#21046#22120
  end
  object Label1: TLabel
    Left = 200
    Top = 136
    Width = 57
    Height = 13
    AutoSize = False
    Caption = #25511#21046#36890#36947
  end
  object Label6: TLabel
    Left = 24
    Top = 101
    Width = 57
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = #27169#24335
  end
  object Label3: TLabel
    Left = 24
    Top = 133
    Width = 57
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = #38480#20301#24320#20851
  end
  object Label4: TLabel
    Left = 200
    Top = 69
    Width = 57
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = #24320#20851#26102#24310
  end
  object Sid: TComboBox
    Left = 263
    Top = 100
    Width = 88
    Height = 21
    ItemHeight = 13
    ItemIndex = 0
    TabOrder = 4
    Text = '1'
    Items.Strings = (
      '1'
      '2'
      '3'
      '4'
      '5'
      '6'
      '7'
      '8')
  end
  object Mode: TComboBox
    Left = 88
    Top = 98
    Width = 88
    Height = 21
    ItemHeight = 13
    TabOrder = 2
    Items.Strings = (
      #26222#36890#24320#20851
      #21040#39034#24320#20851)
  end
  object Chn: TComboBox
    Left = 88
    Top = 66
    Width = 88
    Height = 21
    ItemHeight = 13
    TabOrder = 1
    Items.Strings = (
      '1'
      '2'
      '3'
      '4'
      '5'
      '6'
      '7'
      '8'
      '9'
      '10'
      '11'
      '12'
      '13'
      '14'
      '15'
      '16')
  end
  object Tim: TCSpinEdit
    Left = 264
    Top = 32
    Width = 88
    Height = 22
    TabOrder = 3
  end
  object Button1: TButton
    Left = 216
    Top = 192
    Width = 75
    Height = 25
    Caption = #30830#23450
    ModalResult = 1
    TabOrder = 6
  end
  object Button2: TButton
    Left = 320
    Top = 192
    Width = 75
    Height = 25
    Caption = #21462#28040
    ModalResult = 2
    TabOrder = 7
  end
  object Type: TComboBox
    Left = 88
    Top = 34
    Width = 88
    Height = 21
    ItemHeight = 13
    TabOrder = 0
  end
  object Schn: TComboBox
    Left = 263
    Top = 132
    Width = 88
    Height = 21
    ItemHeight = 13
    TabOrder = 5
    Items.Strings = (
      '1'
      '2'
      '3'
      '4'
      '5'
      '6'
      '7'
      '8'
      '9'
      '10'
      '11'
      '12'
      '13'
      '14'
      '15'
      '16')
  end
  object Limit: TComboBox
    Left = 88
    Top = 130
    Width = 88
    Height = 21
    ItemHeight = 13
    TabOrder = 8
    Items.Strings = (
      #31105#29992
      #38271#24320
      #38271#38381)
  end
  object Delay: TCSpinEdit
    Left = 264
    Top = 64
    Width = 88
    Height = 22
    TabOrder = 9
  end
end
