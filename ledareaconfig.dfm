object areaConfig: TareaConfig
  Left = 759
  Top = 108
  Width = 420
  Height = 292
  Caption = #21306#22495#21442#25968#37197#32622
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 72
    Top = 20
    Width = 36
    Height = 13
    Caption = #32534#21495#65306
  end
  object Label2: TLabel
    Left = 72
    Top = 52
    Width = 43
    Height = 13
    Caption = 'X'#22352#26631#65306
  end
  object Label3: TLabel
    Left = 72
    Top = 76
    Width = 43
    Height = 13
    Caption = 'Y'#22352#26631#65306
  end
  object Label4: TLabel
    Left = 64
    Top = 100
    Width = 60
    Height = 13
    Caption = #21306#22495#23485#24230#65306
  end
  object Label5: TLabel
    Left = 64
    Top = 124
    Width = 60
    Height = 13
    Caption = #21306#22495#39640#24230#65306
  end
  object Label6: TLabel
    Left = 48
    Top = 164
    Width = 84
    Height = 13
    Caption = #21306#22495#25991#23383#34892#25968#65306
  end
  object xPos: TEdit
    Left = 152
    Top = 48
    Width = 57
    Height = 21
    TabOrder = 0
  end
  object yPos: TEdit
    Left = 152
    Top = 72
    Width = 57
    Height = 21
    TabOrder = 1
  end
  object areaW: TEdit
    Left = 152
    Top = 96
    Width = 57
    Height = 21
    TabOrder = 2
  end
  object areaH: TEdit
    Left = 152
    Top = 120
    Width = 57
    Height = 21
    TabOrder = 3
  end
  object Button1: TButton
    Left = 64
    Top = 208
    Width = 75
    Height = 25
    Caption = #30830#23450
    ModalResult = 1
    TabOrder = 4
  end
  object Button2: TButton
    Left = 200
    Top = 208
    Width = 75
    Height = 25
    Caption = #21462#28040
    ModalResult = 2
    TabOrder = 5
  end
  object CmbAreaNo: TComboBox
    Left = 144
    Top = 16
    Width = 57
    Height = 21
    ItemHeight = 13
    TabOrder = 6
    Text = 'CmbAreaNo'
    Items.Strings = (
      '0'
      '1'
      '2'
      '3')
  end
  object CmbTextRows: TComboBox
    Left = 152
    Top = 160
    Width = 57
    Height = 21
    ItemHeight = 13
    TabOrder = 7
    Text = 'CmbTextRows'
    Items.Strings = (
      '1'
      '2'
      '3'
      '4')
  end
end
