object PrintForm: TPrintForm
  Left = 645
  Top = 293
  BorderStyle = bsSingle
  BorderWidth = 4
  Caption = #25171#21360#26426#35774#32622
  ClientHeight = 352
  ClientWidth = 551
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Label14: TLabel
    Left = 16
    Top = 184
    Width = 48
    Height = 13
    Caption = #25171#21360#36895#24230
  end
  object Panel1: TPanel
    Left = -3
    Top = 0
    Width = 556
    Height = 353
    TabOrder = 0
    object GroupPortSel: TGroupBox
      Left = 6
      Top = 8
      Width = 275
      Height = 297
      Caption = #36890#20449#37197#32622
      TabOrder = 0
      object Label1: TLabel
        Left = 8
        Top = 24
        Width = 60
        Height = 13
        Caption = #36873#25321#25171#21360#26426
      end
      object Label2: TLabel
        Left = 22
        Top = 69
        Width = 48
        Height = 13
        Caption = #31471#21475#21517#31216
      end
      object Label3: TLabel
        Left = 44
        Top = 96
        Width = 36
        Height = 13
        Caption = #27874#29305#29575
      end
      object Label4: TLabel
        Left = 45
        Top = 123
        Width = 36
        Height = 13
        Caption = #25968#25454#20301
      end
      object Label5: TLabel
        Left = 55
        Top = 150
        Width = 24
        Height = 13
        Caption = #26657#39564
      end
      object Label6: TLabel
        Left = 43
        Top = 177
        Width = 36
        Height = 13
        Caption = #20572#27490#20301
      end
      object Label7: TLabel
        Left = 46
        Top = 205
        Width = 36
        Height = 13
        Caption = #27969#25511#21046
      end
      object RadioButtonUsb: TRadioButton
        Left = 10
        Top = 232
        Width = 57
        Height = 17
        Caption = 'USB'
        TabOrder = 0
        OnClick = RadioButtonUsbClick
      end
      object RadioButtonCom: TRadioButton
        Left = 10
        Top = 48
        Width = 49
        Height = 17
        Caption = #20018#21475
        TabOrder = 1
        OnClick = RadioButtonComClick
      end
      object RadioButtonLpt: TRadioButton
        Left = 10
        Top = 259
        Width = 57
        Height = 17
        Caption = #24182#21475
        TabOrder = 2
        OnClick = RadioButtonLptClick
      end
      object cbPrinter: TComboBox
        Left = 96
        Top = 16
        Width = 145
        Height = 21
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 3
        Text = 'BTP-2300E'
        Items.Strings = (
          'BTP-2300E')
      end
      object cbComPortName: TComboBox
        Left = 96
        Top = 64
        Width = 145
        Height = 21
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 4
        Text = 'COM1'
        Items.Strings = (
          'COM1'
          'COM2'
          'COM3'
          'COM4'
          'COM5'
          'COM6'
          'COM7'
          'COM8'
          'COM9'
          'COM10'
          'COM11'
          'COM12'
          'COM13'
          'COM14'
          'COM15'
          'COM16'
          'COM17'
          'COM18'
          'COM19'
          'COM20')
      end
      object cbBitRate: TComboBox
        Left = 96
        Top = 91
        Width = 145
        Height = 21
        ItemHeight = 13
        ItemIndex = 3
        TabOrder = 5
        Text = '9600'
        Items.Strings = (
          '1200'
          '2400'
          '4800'
          '9600'
          '19200'
          '38400'
          '57600')
      end
      object cbDataBit: TComboBox
        Left = 96
        Top = 118
        Width = 145
        Height = 21
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 6
        Text = '8'
        Items.Strings = (
          '8')
      end
      object cbCheck: TComboBox
        Left = 96
        Top = 146
        Width = 145
        Height = 21
        ItemHeight = 13
        ItemIndex = 2
        TabOrder = 7
        Text = #26080#26657#39564
        Items.Strings = (
          #20598#26657#39564
          #22855#26657#39564
          #26080#26657#39564)
      end
      object cbStopBit: TComboBox
        Left = 96
        Top = 173
        Width = 145
        Height = 21
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 8
        Text = '1'
        Items.Strings = (
          '1')
      end
      object cbHardCtrl: TComboBox
        Left = 96
        Top = 201
        Width = 145
        Height = 21
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 9
        Text = #26080
        Items.Strings = (
          #26080
          #30828#20214
          #36719#20214)
      end
      object cbUsbSel: TComboBox
        Left = 96
        Top = 228
        Width = 145
        Height = 21
        ItemHeight = 13
        TabOrder = 10
      end
      object cbLptSel: TComboBox
        Left = 96
        Top = 256
        Width = 145
        Height = 21
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 11
        Text = 'LPT1'
        Items.Strings = (
          'LPT1')
      end
    end
    object Button1: TButton
      Left = 96
      Top = 320
      Width = 75
      Height = 25
      Caption = #35774#32622
      TabOrder = 1
      OnClick = Button1Click
    end
    object Button2: TButton
      Left = 384
      Top = 320
      Width = 75
      Height = 25
      Caption = #21462#28040
      TabOrder = 2
      OnClick = Button2Click
    end
    object GroupPortSet: TGroupBox
      Left = 288
      Top = 8
      Width = 257
      Height = 297
      Caption = #25171#21360#37197#32622
      TabOrder = 3
      object Label8: TLabel
        Left = 25
        Top = 29
        Width = 36
        Height = 13
        Caption = #20998#36776#29575
      end
      object Label9: TLabel
        Left = 13
        Top = 60
        Width = 48
        Height = 13
        Caption = #25171#21360#31867#22411
      end
      object Label10: TLabel
        Left = 13
        Top = 92
        Width = 48
        Height = 13
        Caption = #32440#24352#31867#22411
      end
      object Label11: TLabel
        Left = 13
        Top = 124
        Width = 48
        Height = 13
        Caption = #25171#21360#26041#21521
      end
      object Label12: TLabel
        Left = 14
        Top = 156
        Width = 48
        Height = 13
        Caption = #20986#32440#26041#24335
      end
      object Label13: TLabel
        Left = 13
        Top = 188
        Width = 48
        Height = 13
        Caption = #25171#21360#36895#24230
      end
      object Label15: TLabel
        Left = 13
        Top = 220
        Width = 48
        Height = 13
        Caption = #25171#21360#27987#24230
      end
      object cbSize: TComboBox
        Left = 80
        Top = 24
        Width = 145
        Height = 21
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 0
        Text = '300'
        Items.Strings = (
          '300')
      end
      object cbPrintMode: TComboBox
        Left = 80
        Top = 56
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 1
        TabOrder = 1
        Text = #28909#36716#21360
        Items.Strings = (
          #28909#25935
          #28909#36716#21360)
      end
      object cbPaperType: TComboBox
        Left = 80
        Top = 88
        Width = 145
        Height = 21
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 2
        Text = #26631#31614#32440'('#36879#23556')'
        Items.Strings = (
          #26631#31614#32440'('#36879#23556')'
          #36830#32493#32440'('#21453#23556')'
          #40657#26631#35760#32440'('#36879#23556')')
      end
      object cbPrintRever: TComboBox
        Left = 80
        Top = 120
        Width = 145
        Height = 21
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 3
        Text = #26080#26059#36716#25171#21360
        Items.Strings = (
          #26080#26059#36716#25171#21360
          #26059#36716'180'#24230#25171#21360)
      end
      object cbParperOut: TComboBox
        Left = 80
        Top = 152
        Width = 145
        Height = 21
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 4
        Text = #20999#20992#26041#24335
        Items.Strings = (
          #20999#20992#26041#24335
          #21093#31163#26041#24335
          #25749#31163#26041#24335
          #22238#21367#26041#24335
          #21093#31163#26080#30830#35748
          #25749#31163#26080#30830#35748)
      end
      object cbPrintSpeed: TComboBox
        Left = 80
        Top = 184
        Width = 145
        Height = 21
        ItemHeight = 13
        ItemIndex = 2
        TabOrder = 5
        Text = '51mm/s'
        Items.Strings = (
          '25mm/s'
          '38mm/s'
          '51mm/s'
          '63mm/s'
          '76mm/s')
      end
      object cbPrintDensity: TComboBox
        Left = 80
        Top = 216
        Width = 145
        Height = 21
        ItemHeight = 13
        ItemIndex = 25
        TabOrder = 6
        Text = '26'
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
          '16'
          '17'
          '18'
          '19'
          '20'
          '21'
          '22'
          '23'
          '24'
          '25'
          '26'
          '27'
          '28'
          '29'
          '30')
      end
    end
  end
end
