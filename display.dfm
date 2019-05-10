object DisplayForm: TDisplayForm
  Left = 276
  Top = 197
  Width = 704
  Height = 508
  Caption = #35270#39057
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PopupMenu = PopupMenu1
  OnCloseQuery = FormCloseQuery
  OnMouseDown = FormMouseDown
  OnMouseUp = FormMouseUp
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 528
    Height = 450
    Align = alClient
    BevelInner = bvLowered
    BevelOuter = bvLowered
    Color = clBlack
    TabOrder = 0
    OnMouseDown = FormMouseDown
    OnMouseUp = FormMouseUp
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 450
    Width = 688
    Height = 20
    Panels = <
      item
        Text = #31561#24453#21709#24212'...'
        Width = 80
      end
      item
        Text = #30721#27969':'
        Width = 120
      end>
    SimplePanel = False
  end
  object Panel2: TPanel
    Left = 528
    Top = 0
    Width = 160
    Height = 450
    Align = alRight
    BevelOuter = bvNone
    TabOrder = 2
    object Label1: TLabel
      Left = 8
      Top = 80
      Width = 41
      Height = 13
      AutoSize = False
      Caption = #20142'    '#24230
      Transparent = True
    end
    object Label2: TLabel
      Left = 8
      Top = 104
      Width = 41
      Height = 13
      AutoSize = False
      Caption = #23545#27604#24230
      Transparent = True
    end
    object Label3: TLabel
      Left = 8
      Top = 152
      Width = 41
      Height = 13
      AutoSize = False
      Caption = #33394'    '#24230
      Transparent = True
    end
    object Label4: TLabel
      Left = 8
      Top = 128
      Width = 41
      Height = 13
      AutoSize = False
      Caption = #39281#21644#24230
      Transparent = True
    end
    object SpeedButton1: TSpeedButton
      Tag = 9
      Left = 72
      Top = 8
      Width = 23
      Height = 22
      Caption = '<'
      OnMouseDown = SpeedButton1MouseDown
      OnMouseUp = SpeedButton1MouseUp
    end
    object SpeedButton2: TSpeedButton
      Tag = 10
      Left = 104
      Top = 8
      Width = 23
      Height = 22
      Caption = '>'
      OnMouseDown = SpeedButton1MouseDown
      OnMouseUp = SpeedButton1MouseUp
    end
    object SpeedButton3: TSpeedButton
      Tag = 12
      Left = 104
      Top = 40
      Width = 23
      Height = 22
      Caption = '>'
      OnMouseDown = SpeedButton1MouseDown
      OnMouseUp = SpeedButton1MouseUp
    end
    object SpeedButton4: TSpeedButton
      Tag = 11
      Left = 72
      Top = 40
      Width = 23
      Height = 22
      Caption = '<'
      OnMouseDown = SpeedButton1MouseDown
      OnMouseUp = SpeedButton1MouseUp
    end
    object Label5: TLabel
      Left = 8
      Top = 12
      Width = 57
      Height = 13
      AutoSize = False
      Caption = #25918#22823#32553#23567
      Transparent = True
    end
    object Label6: TLabel
      Left = 8
      Top = 44
      Width = 57
      Height = 13
      AutoSize = False
      Caption = #28966#36317#36828#36817
      Transparent = True
    end
    object Label7: TLabel
      Left = 16
      Top = 320
      Width = 60
      Height = 13
      Caption = #25351#23450#39044#32622#20301
    end
    object SpeedButton11: TSpeedButton
      Tag = 1
      Left = 126
      Top = 316
      Width = 20
      Height = 20
      Caption = 'Go'
      OnClick = SpeedButton11Click
    end
    object Label8: TLabel
      Left = 8
      Top = 400
      Width = 57
      Height = 13
      AutoSize = False
      Caption = #36755#20837#38899#37327
      Transparent = True
    end
    object Label9: TLabel
      Left = 8
      Top = 424
      Width = 57
      Height = 13
      AutoSize = False
      Caption = #36755#20986#38899#37327
      Transparent = True
    end
    object Button1: TButton
      Left = 56
      Top = 176
      Width = 89
      Height = 22
      Caption = #32570#30465#21442#25968
      TabOrder = 0
      OnClick = Button1Click
    end
    object TrackBar1: ZTrackBar
      Left = 48
      Top = 76
      Width = 105
      Height = 25
      Max = 255
      Orientation = trHorizontal
      Frequency = 1
      Position = 0
      SelEnd = 0
      SelStart = 0
      TabOrder = 1
      TickMarks = tmBottomRight
      TickStyle = tsNone
      OnMouseUp = TrackBar1MouseUp
    end
    object TrackBar2: ZTrackBar
      Left = 47
      Top = 100
      Width = 105
      Height = 25
      Max = 255
      Orientation = trHorizontal
      Frequency = 1
      Position = 0
      SelEnd = 0
      SelStart = 0
      TabOrder = 2
      TickMarks = tmBottomRight
      TickStyle = tsNone
      OnMouseUp = TrackBar1MouseUp
    end
    object TrackBar3: ZTrackBar
      Left = 47
      Top = 124
      Width = 105
      Height = 25
      Max = 255
      Orientation = trHorizontal
      Frequency = 1
      Position = 0
      SelEnd = 0
      SelStart = 0
      TabOrder = 3
      TickMarks = tmBottomRight
      TickStyle = tsNone
      OnMouseUp = TrackBar1MouseUp
    end
    object TrackBar4: ZTrackBar
      Left = 47
      Top = 148
      Width = 105
      Height = 25
      Max = 255
      Orientation = trHorizontal
      Frequency = 1
      Position = 0
      SelEnd = 0
      SelStart = 0
      TabOrder = 4
      TickMarks = tmBottomRight
      TickStyle = tsNone
      OnMouseUp = TrackBar1MouseUp
    end
    object Button2: TButton
      Left = 16
      Top = 352
      Width = 57
      Height = 22
      Caption = #24320#21551#23545#35762
      TabOrder = 5
      OnClick = Button2Click
    end
    object Button3: TButton
      Left = 88
      Top = 352
      Width = 56
      Height = 22
      Caption = #20572#27490#23545#35762
      TabOrder = 6
      OnClick = Button3Click
    end
    object GroupBox1: TGroupBox
      Left = 16
      Top = 232
      Width = 129
      Height = 81
      Caption = #39044#32622#20301
      TabOrder = 7
      object SpeedButton5: TSpeedButton
        Tag = 1
        Left = 24
        Top = 21
        Width = 20
        Height = 20
        Caption = '1'
        OnClick = SpeedButton5Click
      end
      object SpeedButton6: TSpeedButton
        Tag = 2
        Left = 56
        Top = 21
        Width = 20
        Height = 20
        Caption = '2'
        OnClick = SpeedButton5Click
      end
      object SpeedButton7: TSpeedButton
        Tag = 3
        Left = 88
        Top = 21
        Width = 20
        Height = 20
        Caption = '3'
        OnClick = SpeedButton5Click
      end
      object SpeedButton8: TSpeedButton
        Tag = 4
        Left = 24
        Top = 45
        Width = 20
        Height = 20
        Caption = '4'
        OnClick = SpeedButton5Click
      end
      object SpeedButton9: TSpeedButton
        Tag = 5
        Left = 56
        Top = 45
        Width = 20
        Height = 20
        Caption = '5'
        OnClick = SpeedButton5Click
      end
      object SpeedButton10: TSpeedButton
        Tag = 6
        Left = 88
        Top = 45
        Width = 20
        Height = 20
        Caption = '6'
        OnClick = SpeedButton5Click
      end
    end
    object CheckBox1: TCheckBox
      Left = 88
      Top = 230
      Width = 49
      Height = 17
      Caption = #20445#23384
      TabOrder = 8
    end
    object Preset: TCSpinEdit
      Left = 80
      Top = 316
      Width = 41
      Height = 22
      MaxValue = 255
      MinValue = 1
      TabOrder = 9
      Value = 1
    end
    object PhoneVolum: ZTrackBar
      Left = 64
      Top = 420
      Width = 88
      Height = 25
      Max = 100
      Orientation = trHorizontal
      Frequency = 1
      Position = 0
      SelEnd = 0
      SelStart = 0
      TabOrder = 10
      TickMarks = tmBottomRight
      TickStyle = tsNone
      OnMouseUp = VolumMouseUp
    end
    object MicVolum: ZTrackBar
      Left = 64
      Top = 396
      Width = 88
      Height = 25
      Max = 100
      Orientation = trHorizontal
      Frequency = 1
      Position = 0
      SelEnd = 0
      SelStart = 0
      TabOrder = 11
      TickMarks = tmBottomRight
      TickStyle = tsNone
      OnMouseUp = VolumMouseUp
    end
    object Button4: TButton
      Left = 56
      Top = 200
      Width = 89
      Height = 22
      Caption = #22270#35937#32763#36716
      TabOrder = 12
      OnClick = Button4Click
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 116
    Top = 12
    object N1: TMenuItem
      Caption = #25235#22270
      OnClick = N1Click
    end
  end
end
