import QtQuick
import Quickshell
import Quickshell.Io
import qs.Commons
import qs.Ui
import "Model.js" as BsPatro

// Bikram Sambat (BS) Patro — lightweight BS date pill for the bar (not a
// full calendar): flag + BS day month year, both calendars in the tooltip.
// QML port of bs-patro.c (kept in this repo as the standalone reference);
// the conversion lives in Model.js, unit-tested under plain node.
BarWidget {
  id: root
  moduleName: "kshitij.bs-patro"

  readonly property bool useNp: {
    var v = setting("nepali", false)
    return v === true || v === "true"
  }

  property date today: clock.date

  // Mirror bs-patro.c exactly: one "flag day month year" line horizontal,
  // three stacked lines (flag, day, year) when the bar is vertical — the
  // bar injects `vertical`, so no shell.json scan like the C version needed.
  readonly property var rendered: {
    var d = root.today
    return BsPatro.BsPatroModel.render(
      d.getFullYear(), d.getMonth() + 1, d.getDate(), root.useNp, root.vertical)
  }
  readonly property var displayLines: rendered.text

  SystemClock {
    id: clock
    precision: SystemClock.Minutes
    onDateChanged: root.today = date
  }

  implicitWidth: button.implicitWidth
  implicitHeight: button.implicitHeight

  WidgetButton {
    id: button
    anchors.fill: parent
    bar: root.bar
    text: root.vertical ? "" : root.displayLines.join("\n")
    labelVisible: !root.vertical
    hasVisualContent: root.vertical ? root.displayLines.length > 0 : text !== ""
    fixedHeight: root.vertical ? root.displayLines.length * Style.bar.iconSlot : -1
    horizontalMargin: 8.75
    verticalPadding: 8.75
    tooltipText: root.rendered.tooltip

    Column {
      visible: root.vertical
      anchors.fill: parent

      Repeater {
        model: root.displayLines

        OpticalGlyph {
          required property string modelData
          width: button.width
          height: Style.bar.iconSlot
          text: modelData
          fontFamily: button.fontFamily
          fontSize: modelData.length > 3 ? button.fontSize * 0.9 : button.fontSize
          color: button.foreground
        }
      }
    }
  }
}
