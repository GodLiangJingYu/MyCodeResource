import org.junit.Test;

import static org.junit.Assert.*;

public class DemoTest {

    // ==================== Year Class Tests ====================

    @Test
    public void testYearValidCreation() {
        Year year = new Year(2023);
        assertEquals(2023, year.getYear());
        assertTrue(year.isValid());
    }

    @Test
    public void testYearNegativeCreation() {
        // Assuming negative years are valid for this implementation
        Year year = new Year(-100);
        assertEquals(-100, year.getYear());
        assertTrue(year.isValid());
    }

    @Test(expected = IllegalArgumentException.class)
    public void testYearZeroInvalid() {
        new Year(0);
    }

    @Test
    public void testYearIncrement() {
        Year year = new Year(2023);
        year.increment();
        assertEquals(2024, year.getYear());
    }

    @Test
    public void testYearNegativeIncrement() {
        Year year = new Year(-1);
        year.increment();
        // According to the logic, incrementing from -1 goes to 1, skipping 0.
        assertEquals(1, year.getYear());
    }

    @Test
    public void testYearIncrementFromNegativeOne() {
        Year year = new Year(-1);
        year.increment();
        // According to the logic, incrementing from -1 goes to 1, skipping 0.
        assertEquals(1, year.getYear());
    }

    @Test
    public void testLeapYearPositive() {
        Year year2000 = new Year(2000);
        assertTrue(year2000.isLeap());

        Year year2020 = new Year(2020);
        assertTrue(year2020.isLeap());

        Year year2004 = new Year(2004);
        assertTrue(year2004.isLeap());
    }

    @Test
    public void testNotLeapYearPositive() {
        Year year1900 = new Year(1900);
        assertFalse(year1900.isLeap());

        Year year2023 = new Year(2023);
        assertFalse(year2023.isLeap());

        Year year2100 = new Year(2100);
        assertFalse(year2100.isLeap());
    }

    @Test
    public void testLeapYearNegative() {
        Year yearNeg1 = new Year(-1);
        assertTrue(yearNeg1.isLeap());
        
        Year yearNeg5 = new Year(-5);
        assertTrue(yearNeg5.isLeap());
    }
    
    @Test
    public void testNotLeapYearNegative() {
        Year yearNeg100 = new Year(-100);
        assertFalse(yearNeg100.isLeap());
        
        Year yearNeg4 = new Year(-4);
        assertFalse(yearNeg4.isLeap());
    }

    @Test
    public void testYearEquals() {
        Year year1 = new Year(2023);
        Year year2 = new Year(2023);
        Year year3 = new Year(2024);

        assertTrue(year1.equals(year2));
        assertFalse(year1.equals(year3));
        assertFalse(year1.equals("2023"));
    }

    // ==================== Month Class Tests ====================

    @Test
    public void testMonthValidCreation() {
        Year year = new Year(2023);
        Month month = new Month(6, year);
        assertEquals(6, month.getMonth());
        assertTrue(month.isValid());
    }

    @Test(expected = IllegalArgumentException.class)
    public void testMonthZeroInvalid() {
        Year year = new Year(2023);
        new Month(0, year);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testMonthThirteenInvalid() {
        Year year = new Year(2023);
        new Month(13, year);
    }

    @Test
    public void testMonthIncrement() {
        Year year = new Year(2023);
        Month month = new Month(6, year);
        assertTrue(month.increment());
        assertEquals(7, month.getMonth());
    }

    @Test
    public void testMonthDecemberIncrement() {
        Year year = new Year(2023);
        Month month = new Month(12, year);
        assertFalse(month.increment()); // Should return false as it goes beyond 12
        assertEquals(13, month.getMonth()); // Month should become 13 (beyond valid range)
    }

    @Test
    public void testMonthSizes() {
        Year nonLeap = new Year(2023);
        Month jan = new Month(1, nonLeap);
        assertEquals(31, jan.getMonthSize());

        Month feb = new Month(2, nonLeap);
        assertEquals(28, feb.getMonthSize());

        Month apr = new Month(4, nonLeap);
        assertEquals(30, apr.getMonthSize());
    }

    @Test
    public void testFebruaryLeapYear() {
        Year leap = new Year(2024);
        Month feb = new Month(2, leap);
        assertEquals(29, feb.getMonthSize());
    }

    @Test
    public void testMonthEquals() {
        Year year = new Year(2023);
        Month month1 = new Month(6, year);
        Month month2 = new Month(6, new Year(2023));
        Month month3 = new Month(7, year);

        assertTrue(month1.equals(month2));
        assertFalse(month1.equals(month3));
        assertFalse(month1.equals("6"));
    }

    // ==================== Day Class Tests ====================

    @Test
    public void testDayValidCreation() {
        Year year = new Year(2023);
        Month month = new Month(6, year);
        Day day = new Day(15, month);
        assertEquals(15, day.getDay());
        assertTrue(day.isValid());
    }

    @Test(expected = IllegalArgumentException.class)
    public void testDayZeroInvalid() {
        Year year = new Year(2023);
        Month month = new Month(6, year);
        new Day(0, month);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testDayThirtyTwoInJuneInvalid() {
        Year year = new Year(2023);
        Month month = new Month(6, year);
        new Day(32, month);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testDayThirtyOneInAprilInvalid() {
        Year year = new Year(2023);
        Month month = new Month(4, year);
        new Day(31, month);
    }

    @Test
    public void testDayFebruaryLeapYear() {
        Year leapYear = new Year(2024);
        Month feb = new Month(2, leapYear);
        Day day29 = new Day(29, feb);
        assertEquals(29, day29.getDay());
        assertTrue(day29.isValid());
    }

    @Test(expected = IllegalArgumentException.class)
    public void testDayFebruaryNonLeapYear() {
        Year nonLeapYear = new Year(2023);
        Month feb = new Month(2, nonLeapYear);
        new Day(29, feb);
    }

    @Test
    public void testDayIncrement() {
        Year year = new Year(2023);
        Month month = new Month(6, year);
        Day day = new Day(15, month);
        assertTrue(day.increment());
        assertEquals(16, day.getDay());
    }

    @Test
    public void testDayEndOfMonthIncrement() {
        Year year = new Year(2023);
        Month month = new Month(6, year);
        Day day = new Day(30, month);
        assertFalse(day.increment()); // Should return false as it goes beyond month size
        assertEquals(31, day.getDay()); // Day should become 31 (beyond valid range)
    }

    @Test
    public void testDayEquals() {
        Year year = new Year(2023);
        Month month = new Month(6, year);
        Day day1 = new Day(15, month);
        Day day2 = new Day(15, new Month(6, new Year(2023)));
        Day day3 = new Day(16, month);

        assertTrue(day1.equals(day2));
        assertFalse(day1.equals(day3));
        assertFalse(day1.equals("15"));
    }

    // ==================== Date Class Tests ====================

    @Test
    public void testDateValidCreation() {
        Date date = new Date(6, 15, 2023);
        assertEquals(6, date.getMonth().getMonth());
        assertEquals(15, date.getDay().getDay());
        assertEquals(2023, date.getYear().getYear());
    }

    @Test
    public void testDateIncrementNormalDay() {
        Date date = new Date(6, 15, 2023);
        date.increment();
        assertEquals(6, date.getMonth().getMonth());
        assertEquals(16, date.getDay().getDay());
        assertEquals(2023, date.getYear().getYear());
    }

    @Test
    public void testDateIncrementEndOfMonth() {
        Date date = new Date(6, 30, 2023);
        date.increment();
        assertEquals(7, date.getMonth().getMonth());
        assertEquals(1, date.getDay().getDay());
        assertEquals(2023, date.getYear().getYear());
    }

    @Test
    public void testDateIncrementEndOfYear() {
        Date date = new Date(12, 31, 2023);
        date.increment();
        assertEquals(1, date.getMonth().getMonth());
        assertEquals(1, date.getDay().getDay());
        assertEquals(2024, date.getYear().getYear());
    }

    @Test
    public void testDateIncrementLeapYear() {
        Date date = new Date(2, 28, 2024);
        date.increment();
        assertEquals(2, date.getMonth().getMonth());
        assertEquals(29, date.getDay().getDay());
        assertEquals(2024, date.getYear().getYear());

        date.increment();
        assertEquals(3, date.getMonth().getMonth());
        assertEquals(1, date.getDay().getDay());
        assertEquals(2024, date.getYear().getYear());
    }

    @Test
    public void testDateIncrementNonLeapYear() {
        Date date = new Date(2, 28, 2023);
        date.increment();
        assertEquals(3, date.getMonth().getMonth());
        assertEquals(1, date.getDay().getDay());
        assertEquals(2023, date.getYear().getYear());
    }

    @Test
    public void testDateEquals() {
        Date date1 = new Date(6, 15, 2023);
        Date date2 = new Date(6, 15, 2023);
        Date date3 = new Date(6, 16, 2023);

        assertTrue(date1.equals(date2));
        assertFalse(date1.equals(date3));
        assertFalse(date1.equals("6/15/2023"));
    }

    @Test
    public void testDateToString() {
        Date date = new Date(6, 15, 2023);
        assertEquals("6/15/2023", date.toString());
    }

    // ==================== NextDay Class Tests ====================

    @Test
    public void testNextDayNormal() {
        Date input = new Date(6, 15, 2023);
        Date result = Nextday.nextDay(input);
        Date expected = new Date(6, 16, 2023);

        assertTrue(expected.equals(result));
    }

    @Test
    public void testNextDayEndOfMonth() {
        Date input = new Date(6, 30, 2023);
        Date result = Nextday.nextDay(input);
        Date expected = new Date(7, 1, 2023);

        assertTrue(expected.equals(result));
    }

    @Test
    public void testNextDayEndOfYear() {
        Date input = new Date(12, 31, 2023);
        Date result = Nextday.nextDay(input);
        Date expected = new Date(1, 1, 2024);

        assertTrue(expected.equals(result));
    }

    @Test
    public void testNextDayFebruaryLeapYear() {
        Date input = new Date(2, 28, 2024);
        Date result = Nextday.nextDay(input);
        Date expected = new Date(2, 29, 2024);

        assertTrue(expected.equals(result));
    }

    @Test
    public void testNextDayFebruary29ToMarch1() {
        Date input = new Date(2, 29, 2024);
        Date result = Nextday.nextDay(input);
        Date expected = new Date(3, 1, 2024);

        assertTrue(expected.equals(result));
    }

    @Test
    public void testNextDayFebruaryNonLeapYear() {
        Date input = new Date(2, 28, 2023);
        Date result = Nextday.nextDay(input);
        Date expected = new Date(3, 1, 2023);

        assertTrue(expected.equals(result));
    }

    @Test
    public void testNextDayDoesNotModifyOriginal() {
        Date input = new Date(6, 15, 2023);
        Date original = new Date(6, 15, 2023);

        Date result = Nextday.nextDay(input);

        assertTrue(original.equals(input));
        assertFalse(result.equals(input));
    }

    // ==================== Edge Cases ====================

    @Test
    public void testDateBoundaryJanuary1() {
        Date date = new Date(1, 1, 2023);
        date.increment();
        assertEquals(1, date.getMonth().getMonth());
        assertEquals(2, date.getDay().getDay());
        assertEquals(2023, date.getYear().getYear());
    }

    @Test
    public void testDateBoundaryDecember31() {
        Date date = new Date(12, 31, 2023);
        date.increment();
        assertEquals(1, date.getMonth().getMonth());
        assertEquals(1, date.getDay().getDay());
        assertEquals(2024, date.getYear().getYear());
    }

    @Test
    public void testNegativeYear() {
        Date date = new Date(6, 15, -100);

        assertEquals(-100, date.getYear().getYear());
        assertTrue(date.getYear().isValid());
        assertTrue(date.getMonth().isValid());
        assertTrue(date.getDay().isValid());
    }

    @Test
    public void testAllMonthsHaveValidDays() {
        Year year = new Year(2023);

        for (int month = 1; month <= 12; month++) {
            Month m = new Month(month, year);
            int maxDay = m.getMonthSize();

            Day firstDay = new Day(1, m);
            assertEquals(1, firstDay.getDay());

            Day lastDay = new Day(maxDay, m);
            assertEquals(maxDay, lastDay.getDay());
        }
    }

    @Test
    public void testMonthWithThirtyDays() {
        Year year = new Year(2023);
        Month apr = new Month(4, year);
        Day apr30 = new Day(30, apr);
        assertTrue(apr30.isValid());
    }

    @Test
    public void testMonthWithThirtyOneDays() {
        Year year = new Year(2023);
        Month jan = new Month(1, year);
        Day jan31 = new Day(31, jan);
        assertTrue(jan31.isValid());
    }

    @Test
    public void testYearEqualsWithNull() {
        Year year1 = new Year(2023);
        assertFalse(year1.equals(null));
    }
    
    @Test
    public void testMonthEqualsWithNull() {
        Year year = new Year(2023);
        Month month = new Month(6, year);
        assertFalse(month.equals(null));
    }
    
    @Test
    public void testDayEqualsWithNull() {
        Year year = new Year(2023);
        Month month = new Month(6, year);
        Day day = new Day(15, month);
        assertFalse(day.equals(null));
    }
    
    @Test
    public void testDateEqualsWithNull() {
        Date date = new Date(6, 15, 2023);
        assertFalse(date.equals(null));
    }

    @Test
    public void testYearSetInvalidYear() {
        try {
            Year year = new Year(2023);
            year.setYear(0); // This should throw exception
            fail("Should have thrown IllegalArgumentException");
        } catch (IllegalArgumentException e) {
            assertEquals("Not a valid month", e.getMessage());
        }
    }
    
    @Test
    public void testMonthSetInvalidMonth() {
        Year year = new Year(2023);
        try {
            Month month = new Month(6, year);
            month.setMonth(0, year); // This should throw exception
            fail("Should have thrown IllegalArgumentException");
        } catch (IllegalArgumentException e) {
            assertEquals("Not a valid month", e.getMessage());
        }
    }
    
    @Test
    public void testDaySetInvalidDay() {
        Year year = new Year(2023);
        Month month = new Month(6, year);
        try {
            Day day = new Day(15, month);
            day.setDay(0, month); // This should throw exception
            fail("Should have thrown IllegalArgumentException");
        } catch (IllegalArgumentException e) {
            assertEquals("Not a valid day", e.getMessage());
        }
    }
    
    @Test
    public void testMonthFebSizeWithLeapYearEdge() {
        Year leapYear = new Year(2000); // Divisible by 400
        Month feb = new Month(2, leapYear);
        assertEquals(29, feb.getMonthSize());
        
        Year nonLeapCentury = new Year(1900); // Divisible by 100 but not 400
        Month febNonLeap = new Month(2, nonLeapCentury);
        assertEquals(28, febNonLeap.getMonthSize());
    }
    
    @Test
    public void testYearIncrementEdgeCases() {
        // Test increment from -1 to 0 to 1 edge case
        Year year = new Year(-1);
        assertEquals(-1, year.getYear());
        
        year.increment();
        assertEquals(1, year.getYear()); // Should become 0 then 1 due to special logic
        
        // Test normal increment
        Year normalYear = new Year(1);
        normalYear.increment();
        assertEquals(2, normalYear.getYear());
    }
}
