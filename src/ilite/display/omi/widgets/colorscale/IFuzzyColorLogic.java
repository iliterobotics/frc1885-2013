package ilite.display.omi.widgets.colorscale;

import ilite.display.omi.widgets.IColorScale;
import ilite.util.lang.IPredicate;

/**
 * @author JesseK
 *
 *  Fuzzy-logic-esque interface used to predicate whether the colorscale method
 *  should be used.  Mainly meant for use in composite colorscales which need
 *  to separate the range check from the color generation.  Composites allow
 *  a general colorscale to be used in general, with special cases taking over
 *  under the right conditions.
 *  
 * <p> This is an interface rather than a class so that it can be implemented
 * in an enumeration.
 *
 * @param <T>
 */
public interface IFuzzyColorLogic<T> extends IColorScale<T>, IPredicate<T>
{
}
