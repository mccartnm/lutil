
from . import cpp_syntax
from ..style import Colors, Sizes

from PySide2 import QtWidgets, QtCore, QtGui
from PySide2.QtCore import Qt

class _LineNumbers(QtWidgets.QWidget):
    """
    Helper widget for painting the line number(s) of the
    widget below.
    """
    def __init__(self, editor=None):
        super(_LineNumbers, self).__init__(editor)
        self._editor = editor


    def sizeHint(self):
        return QtCore.QSize(self._editor.lineNumberAreaWidth(), 0)


    def paintEvent(self, event):
        """ Redirect to the text edit """
        self._editor.line_number_area_paint_event(event)



class CppTextEdit(QtWidgets.QPlainTextEdit):
    """
    Plain text editor that we use for editing python within
    the application
    """
    space = ' '
    tab_space = 4

    # -- Keys that we have "surround" logic for
    surrounding_keys = {
        Qt.Key_QuoteDbl: '"',
        Qt.Key_Apostrophe: "'",
        Qt.Key_ParenLeft: ')',
        Qt.Key_BracketLeft: ']',
        Qt.Key_BraceLeft: '}'
    }

    def __init__(self, parent=None):
        super().__init__(parent)

        self.colors = Colors()
        self._line_numbers = _LineNumbers(self)

        font = QtGui.QFont("Courier New")
        font.setStyleHint(QtGui.QFont.Monospace)
        self.setFont(font)
        
        p = self.palette()
        p.setColor(QtGui.QPalette.Base, self.colors.TextBackground)
        p.setColor(QtGui.QPalette.Window, self.colors.Background)
        self.setPalette(p)
        self.setBackgroundVisible(True)
        self.setAutoFillBackground(True)

        self.setStyleSheet("border: 1px solid black; background-color: {};"
                           .format(self.colors.TextBackground.name()))

        self.blockCountChanged.connect(self.update_line_number_area_width)
        self.updateRequest[(QtCore.QRect,int)].connect(self.update_line_number_area)
        self.cursorPositionChanged.connect(self.highlight_current_line)

        self.update_line_number_area_width(0)
        self.highlight_current_line()


    def to_code(self, force_all=False):
        """
        If there is a selection, return that, otherwise return all the text

        :param force_all: ``bool`` - When True, ignore selected text
        :return: ``str``
        """
        if not force_all:
            sel = self.textCursor().selectedText()
            if sel:
                # https://doc.qt.io/qt-5/qtextcursor.html#selectedText
                return sel.replace('\u2029', '\n')
        return self.toPlainText()


    def line_number_area_paint_event(self, event):
        """
        Paint the line numbers for this editor
        
        :param event: ``QPaintEvent``
        :return: ``None``
        """
        p = QtGui.QPainter(self._line_numbers)
        p.fillRect(event.rect(), "#222222")

        block = self.firstVisibleBlock();
        blockNumber = block.blockNumber();
        top = int(
            self.blockBoundingGeometry(
                block
            ).translated(self.contentOffset()).top()
        )
        bottom = top + int(self.blockBoundingRect(block).height())

        fm = self.fontMetrics();
        buff = 10

        while block.isValid() and top <= event.rect().bottom():

            if block.isVisible() and bottom >= event.rect().top():
                number = str((blockNumber + 1))
                p.setPen("#888888")
                p.drawText(
                    0,
                    top,
                    self._line_numbers.width() - buff,
                    fm.height(),
                    Qt.AlignRight,
                    number
                )


            block = block.next()
            top = bottom
            bottom = top + int(self.blockBoundingRect(block).height())
            blockNumber += 1


    def line_number_area_width(self):
        """
        Based on the number of lines, we have the line
        numbers display as required
        """
        digits = 1
        max_ = max(1, self.blockCount())
        while max_ >= 10:
            digits += 1
            max_ = max_ // 10

        fm = self.fontMetrics()
        space = fm.horizontalAdvance('9') * digits
        return space + Sizes.EditorLineNumberPadding


    def update_line_number_area_width(self, new_block_count):
        """
        Based on the number of digits, set the proper size of the margins
        to account for the editor
        """
        self.setViewportMargins(self.line_number_area_width(), 0, 0, 0)


    def update_line_number_area(self, line_rect, dy):
        """
        Update
        """
        if dy:
            self._line_numbers.scroll(0, dy)
        else:
            self._line_numbers.update(
                0, line_rect.y(),
                self._line_numbers.width(), line_rect.height()
            )

        if line_rect.contains(self.viewport().rect()):
            self.update_line_number_area_width(0)


    def highlight_current_line(self):
        """
        Highlight the current line based on the cursor position
        """
        extra_selections = []
        if self.isReadOnly():
            p = self.palette()
            sel = QtWidgets.QTextEdit.ExtraSelection()
            line_color = p.color(p.Window).lighter(100)

            sel.format.setBackground(line_color)
            sel.format.setProperty(
                QtWidgets.QTextFormat.FullWidthSelection, True
            )
            sel.cursor = self.textCursor()
            sel.cursor.clearSelection()
            extra_selections.append(sel)
        self.setExtraSelections(extra_selections)


    def keyPressEvent(self, event):
        """
        Handle various "hotkeys"
        """
        if event.key() == Qt.Key_Tab:
            line = self.line_text()
            self.insertPlainText(
                self.space * (self.tab_space -\
                (len(line) %  self.tab_space))
            )

        elif event.key() in (Qt.Key_Enter, Qt.Key_Return):
            super().keyPressEvent(event)
            pline = self.textCursor().block().previous().text()
            eline = self.textCursor().block().text()

            if pline and not any(c != self.space for c in pline):
                # -- All whitespace, clean that up to avoid ugly
                #    spaces everywhere
                cursor = self.textCursor()
                cursor.movePosition(QtGui.QTextCursor.PreviousBlock)
                cursor.select(QtGui.QTextCursor.LineUnderCursor)
                cursor.removeSelectedText()

            indent_count = 0
            additional_text = None

            expression = QtCore.QRegExp(r"^\s+")
            pos = expression.indexIn(pline)

            braces = ('{', '(', '[')
            revbraces = ('}', ')', ']')

            if pos > -1:
                # -- We have some spaces in front of our last line
                indent_count = len(expression.cap(0))

            if pline.endswith(":"):
                # -- Attempt jumping up an indent
                indent_count += self.tab_space

            elif (pline and pline[-1] in braces) and (eline and eline[0] in revbraces):
                additional_text = (self.space * self.tab_space) + '\n' + (self.space * indent_count)

            if indent_count:
                self.insertPlainText(
                    self.space * indent_count
                )

            if additional_text:
                self.insertPlainText(additional_text)
                self.moveCursor(QtGui.QTextCursor.PreviousBlock)
                self.moveCursor(QtGui.QTextCursor.EndOfBlock)

        elif event.key() in self.surrounding_keys:
            # -- If we have a selection, we surround the values with
            cursor = self.textCursor()
            if cursor.hasSelection():
                q = event.text()
                start = cursor.selectionStart()
                end = cursor.selectionEnd()
                cursor.beginEditBlock()
                cursor.setPosition(start)
                cursor.insertText(q)
                cursor.setPosition(end + 1) # Account for the new qoute
                cursor.insertText(self.surrounding_keys[event.key()])

                cursor.clearSelection()
                cursor.setPosition(start + 1)
                cursor.setPosition(end + 1, QtGui.QTextCursor.KeepAnchor)
                cursor.endEditBlock()
                self.setTextCursor(cursor)
            else:
                super().keyPressEvent(event)

        elif event.key() == Qt.Key_Backspace:
            # -- Clean back spaced based on the text size.
            line = self.line_text()
            sel = self.textCursor().selectedText()
            if sel or any(c != self.space for c in line) or len(line) == 0:
                # This isn't whitespace only
                super().keyPressEvent(event)
                return
            space_count = len(line)
            to_backup = (space_count % self.tab_space) or self.tab_space

            for i in range(to_backup):
                # -- Backup as mny times as needed
                super().keyPressEvent(event)

        else:
            super().keyPressEvent(event)


    def resizeEvent(self, event):
        """
        We need to propogate the resizing to the line numbers
        properly
        """
        super().resizeEvent(event)
        cr = self.contentsRect()
        self._line_numbers.setGeometry(
            QtCore.QRect(
                cr.left(),
                cr.top(),
                self.line_number_area_width(),
                cr.height()
            )
        )


    def line_text(self):
        """
        Obtain the current text of this line
        """
        cursor = self.textCursor()
        block = cursor.block()
        return block.text()
